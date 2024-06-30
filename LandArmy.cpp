#include "LandArmy.h"
#include "Territory.h"
#include "IOccupiable.h"
#include "TextureManager.h"
#include "Player.h"
#include "GameplaySettings.h"
#include "Terrain.h"
#include "LandTerritory.h"
#include <assert.h>
#include <random>
#include <iostream>

LandArmy::LandArmy(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, "circle")
{
	assert(location != nullptr);
}

LandArmy::LandArmy(Player & owner, Territory * location, StaminaArray staminaStrength)
	: MilitaryForce(owner, location, staminaStrength, "circle")
{
	assert(location != nullptr);
}

void LandArmy::removeFromTerritory()
{
	// Remove the army from territory and updates the occupancy.
	getTerritory().getOccupancyHandler()->removeArmy(this);
}

void LandArmy::attack(LandArmy &defendingArmy, const Terrain &terrain)
{
	const GameplaySettings &gameplaySettings = getOwner().getGameplaySettings();

	assert(getTotalStrength() >= gameplaySettings.landHostileOccupancyCost);

	// Strength multipliers.
	double defenceMultiplier = terrain.defenceMultiplier;
	double attackMultiplier = terrain.attackMultiplier;

	// Determine adjusted strength of both attacker and defender.
	const double defenderStrength = static_cast<double>(defendingArmy.getTotalStrength());
	const double defenderAdjustedStrength = defenderStrength * defenceMultiplier;  
	const double attackerStrength = getTotalStrength();
	const double attackerAdjustedStrength = attackerStrength * attackMultiplier; 
	assert(defenderStrength > 0 && defenderAdjustedStrength > 0);
	assert(attackerStrength > 0 && attackerAdjustedStrength > 0);

	// Max and min percent of total strength armies lose.
	const double adjustedStrengthsRatio = attackerAdjustedStrength / defenderAdjustedStrength;
	const double maxMultiplierFactor = 0.4 * gameplaySettings.lossMultiplier;
	const double minMultiplierFactor = 0.2 * gameplaySettings.lossMultiplier;
	const double maxDefenderLossMultiplier = maxMultiplierFactor * adjustedStrengthsRatio;
	const double minDefenderLossMultiplier = minMultiplierFactor * adjustedStrengthsRatio;
	const double maxAttackerLossMultiplier = maxMultiplierFactor / adjustedStrengthsRatio;
	const double minAttackerLossMultiplier = minMultiplierFactor / adjustedStrengthsRatio;

	// Calculate strength adjustments. Partially randomised.
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<double> defenderLossDist(minDefenderLossMultiplier, maxDefenderLossMultiplier);  // Percent of strength defending army loses.
	std::uniform_real_distribution<double> attackerLossDist(minAttackerLossMultiplier, maxAttackerLossMultiplier);  // Percent of strength attacking army loses.
	int defenderStrengthAdjustment = std::round(defenderLossDist(rng) * defenderAdjustedStrength);
	int attackerStrengthAdjustment = std::round(attackerLossDist(rng) * attackerAdjustedStrength);

	assert(defenderStrengthAdjustment >= 0);
	assert(attackerStrengthAdjustment >= 0);

	// Special case where no damage was calculated for either army.
	if(defenderStrengthAdjustment == 0 && attackerStrengthAdjustment == 0)
	{
		// Attacker takes guaranteed damage.
		++attackerStrengthAdjustment;
		
		// Apply damage to weaker army.
		if(attackerAdjustedStrength < defenderAdjustedStrength)
		{
			++attackerStrengthAdjustment;
		}
		else if(attackerAdjustedStrength > defenderAdjustedStrength)
		{
			++defenderStrengthAdjustment;
		}
	}

	// Apply reductions to strength of armies.
	defendingArmy.reduceStrength(defenderStrengthAdjustment);
	reduceStrength(attackerStrengthAdjustment);
}

int LandArmy::getLandMoveStaminaCost() 
{
	return MAX_STAMINA;
}

int LandArmy::getAttackStaminaCost()
{
	return MAX_STAMINA;
}

std::pair<int, int> LandArmy::calculateMinMaxStaminaCost(const Territory &territory) const
{
	if(getTerritory().getType() == TerritoryType::land || territory.getType() == TerritoryType::land)
	{
		return { MAX_STAMINA, MAX_STAMINA };
	}
	else
	{
		return { MAX_STAMINA, MAX_STAMINA };
	}
}

void LandArmy::move(Territory &location, unsigned int strength)
{
	assert(strength <= getTotalStrength());

	const GameplaySettings &gameplaySettings = getOwner().getGameplaySettings();

	if(strength == 0)
	{
		return;
	}

	// Strength must exceed min hostile occupancy cost when moving to enemy or unowned location.
	if(!sameUpperRealm(&getOwner(), location.getEstateOwner()) && strength < gameplaySettings.landHostileOccupancyCost)
	{
		return;
	}

	// Attempt to move to current location.
	if(&location == &getTerritory())
	{
		return;
	}

	// Attempt to move to non adjacent location.
	if(!getTerritory().getDistanceMap().isAdjacent(&location))
	{
		return;
	}

	// Determine stamina strength array of army being moved.
	// It is possible that the total strength of this army is 0. We do not yet trigger any death related events
	// however as it is possible for strength to be refunded to this army.
	std::array<unsigned int, 3> expendedStrength = expendStrength(strength, location);
	// Land army attempting location occupation.
	std::unique_ptr<LandArmy> newArmy = std::make_unique<LandArmy>(getOwner(), &getTerritory(), expendedStrength);  
	// Only proceed if new army strength is greater than 0.
	if(newArmy.get()->isDead())
	{
		return;
	}

	// Update diplomacy of involved players.
	updatePlayerDiplomacy(location.getEstateOwner());

	// Attempt occupation of location by new army.
	location.getOccupancyHandler()->occupy(newArmy.get());

	// Refund strength to this->army if deployedArmy is not able to occupy location
	if(&newArmy.get()->getTerritory() == &getTerritory())
	{
		std::array<unsigned int, 3> strengthRefund = newArmy.get()->getStaminaStrength();
		increaseStrength(strengthRefund);
		newArmy.reset();
	}
	else
	{
		// Add the army to the military manager of owning player.
		if(!newArmy.get()->isDead())
		{
			getOwner().getMilitaryManager().addLandArmy(std::move(newArmy));
		}
	}

	// After any strength refunds we check if the army is dead to trigger death related events.
	checkDeath();
}

void LandArmy::moveClosest(Territory &target, unsigned int strength, int maxDist)
{
	assert(strength > 0);
	// std::set<Territory*> adjacencies = getTerritory().getDistanceMap().getAdjacencies();
	// // Construct adjacent territories with same owner as army owner.
	// std::set<Territory*> friendlyAdjacencies;  
	// for(Territory* territory : adjacencies)
	// {
	// 	if(territory->getEstateOwner() == &getOwner())
	// 	{
	// 		friendlyAdjacencies.insert(territory);
	// 	}
	// }

	Territory& source = getTerritory();
	Territory* nearest = nearestFriendlyAdjacentTerritoryDijkstra(source, target, maxDist);
	if(nearest != nullptr)
	{
		move(*nearest, strength);
	}

	//////////////////
	// in future prioritise border territories PERHAPS!
	//////////////
}


