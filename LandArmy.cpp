#include "LandArmy.h"
#include "Territory.h"
#include "TextureManager.h"
#include "Player.h"
#include <assert.h>
#include <random>
#include <iostream>

LandArmy::LandArmy(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, "circle")
{
	assert(location != nullptr);
}

LandArmy::LandArmy(Player & owner, Territory * location, std::array<unsigned int, 4> staminaStrength)
	: MilitaryForce(owner, location, staminaStrength, "circle")
{
	assert(location != nullptr);
}

void LandArmy::attack(LandArmy &defendingArmy, double defenceMultiplier)
{
	const double defenderStrength = static_cast<double>(defendingArmy.getTotalStrength());
	const double defenderAdjustedStrength = defenderStrength * defenceMultiplier;  // defender strength.
	const double attackerStrength = getTotalStrength();
	assert(defenderStrength > 0);
	assert(attackerStrength > 0);

	int defenderStrengthAdjustment = 0;
	int attackerStrengthAdjustment = 0;

	// Adjust strengths for passing a given threshold.
	const double strengthThreshold = 3;  // Threshold for guaranteed damage on opponent.
	const int thresholdAdjustmentValue = 1;  // Strength adjustment for opponent passing threshold.
	if(defenderAdjustedStrength >= strengthThreshold)
	{
		attackerStrengthAdjustment += thresholdAdjustmentValue;
	}
	if(attackerStrength >= strengthThreshold)
	{
		defenderStrengthAdjustment += thresholdAdjustmentValue;
	}

	// Partially randomised damage to each army.
	const double strengthRatio = attackerStrength / defenderAdjustedStrength;
	const double maxMultiplier = 0.4;
	const double minMultiplier = 0.2;
	const double maxAttacker = maxMultiplier * strengthRatio;
	const double minAttacker = minMultiplier * strengthRatio;
	const double maxDefender = maxMultiplier / strengthRatio;
	const double minDefender = minMultiplier / strengthRatio;
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<double> attackerDist(minAttacker, maxAttacker);  // Percent of strength defending army loses.
	std::uniform_real_distribution<double> defenderDist(minDefender, maxDefender);  // Percent of strength attacking army loses.
	defenderStrengthAdjustment += std::round(attackerDist(rng) * defenderStrength);
	attackerStrengthAdjustment += std::round(defenderDist(rng) * attackerStrength);
	assert(defenderStrengthAdjustment >= 0);
	assert(attackerStrengthAdjustment >= 0);
	defendingArmy.reduceStrength(defenderStrengthAdjustment);
	reduceStrength(attackerStrengthAdjustment);

	// Special case where no damage is taken by either army.
	if(getTotalStrength() == attackerStrength && defendingArmy.getTotalStrength() == defenderStrength)
	{
		assert(attackerStrength < strengthThreshold && defenderStrength < strengthThreshold);
		assert(attackerStrengthAdjustment == 0 && defenderStrengthAdjustment == 0);

		// Determine strength adjustment.
		std::uniform_int_distribution<int> dist(0, 1);
		if(defenderAdjustedStrength > attackerStrength)
		{
			++attackerStrengthAdjustment;
		}
		else if(defenderAdjustedStrength < attackerStrength)
		{
			++defenderStrengthAdjustment;
		}
		else if(dist(rng) == 1)
		{
			++attackerStrengthAdjustment;
		}
		else
		{
			++defenderStrengthAdjustment;
		}

		// Adjust army strengths.
		defendingArmy.reduceStrength(defenderStrengthAdjustment);
		reduceStrength(attackerStrengthAdjustment);
	}
}

std::pair<int, int> LandArmy::calculateMinMaxStaminaCost(const Territory &territory) const
{
	if(getTerritory().getType() == TerritoryType::land || territory.getType() == TerritoryType::land)
	{
		return { 3, 3 };
	}
	else
	{
		return { 1, 2 };
	}
}

void LandArmy::move(Territory &location, unsigned int strength)
{
	assert(strength > 0);
	assert(strength <= getTotalStrength());

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
	std::array<unsigned int, 4> expendedStrength = expendStrength(strength, location);
	// Land army attempting location occupation.
	std::unique_ptr<LandArmy> newArmy = std::make_unique<LandArmy>(getOwner(), &getTerritory(), expendedStrength);  
	// Only proceed if new army strength is greater than 0.
	if(newArmy.get()->isDead())
	{
		return;
	}

	// Attempt occupation of location by new army.
	location.getOccupancyHandler()->occupy(newArmy.get());

	// Refund strength to this->army if deployedArmy is not able to occupy location
	if(&newArmy.get()->getTerritory() == &getTerritory())
	{
		std::array<unsigned int, 4> strengthRefund = newArmy.get()->getStaminaStrength();
		increaseStrength(strengthRefund);
		newArmy.reset();
	}
	else
	{
		// Add the army to the military manager of owning player.
		getOwner().getMilitaryManager().addLandArmy(std::move(newArmy));
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


