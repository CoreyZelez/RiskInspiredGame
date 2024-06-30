#include "NavalFleet.h"
#include "Territory.h"
#include "Player.h"
#include "TextureManager.h"
#include "Territory.h"
#include <assert.h>
#include <memory>
#include <random>
#include <iostream>

NavalFleet::NavalFleet(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, "triangle")
{
	assert(location != nullptr);
}

NavalFleet::NavalFleet(Player & owner, Territory * location, std::array<unsigned int, 3> staminaStrength)
	: MilitaryForce(owner, location, staminaStrength, "triangle")
{
	assert(location != nullptr);
}

void NavalFleet::removeFromTerritory()
{
	// Remove the fleet from territory and update the occupancy.
	getTerritory().getOccupancyHandler()->removeFleet(this);
}

void NavalFleet::move(Territory &location, unsigned int strength)
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

	// Determine stamina strength array of fleet being moved.
	// It is possible that the total strength of this fleet is 0. We do not yet trigger any death related events
	// however as it is possible for strength to be refunded to this fleet.
	std::array<unsigned int, 3> expendedStrength = expendStrength(strength, location);
	// Naval fleet attempting location occupation.
	std::unique_ptr<NavalFleet> newFleet = std::make_unique<NavalFleet>(getOwner(), &getTerritory(), expendedStrength);
	// Only proceed if new army strength is greater than 0.
	if(newFleet.get()->isDead())
	{
		return;
	}

	// Update diplomacy of involved players.
	updatePlayerDiplomacy(location.getEstateOwner());

	// Attempt occupation of location by new fleet.
	location.getOccupancyHandler()->occupy(newFleet.get());

	// Refund strength to this->fleet if deployed fleet is not able to occupy location
	if(&newFleet.get()->getTerritory() == &getTerritory())
	{
		std::array<unsigned int, 3> strengthRefund = newFleet.get()->getStaminaStrength();
		increaseStrength(strengthRefund);
		newFleet.reset();
	}
	else
	{
		// Add the army to the military manager of owning player.
		getOwner().getMilitaryManager().addNavalFleet(std::move(newFleet));
	}

	// After any strength refunds we check if the army is dead to trigger death related events.
	checkDeath();
}

void NavalFleet::moveClosest(Territory &target, unsigned int strength, int maxDist)
{
	assert(strength > 0);
	
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

void NavalFleet::attack(NavalFleet &defendingNavy, double defenceMultiplier)
{
	const double defenderTrueStrength = static_cast<double>(defendingNavy.getTotalStrength());  // Defender strength before adjustments.
	const double defenderStrength = defenderTrueStrength * defenceMultiplier;  // defender strength adjusted for defence multiplier.
	const double attackerStrength = getTotalStrength();
	int attackerStrengthAdjustment = 0;
	int defenderStrengthAdjustment = 0;
	assert(defenderTrueStrength > 0);
	assert(defenderStrength > 0);
	assert(attackerStrength > 0);

	// Apply guaranteed damage.
	const float guaranteedDamageRatio = 0.2f;
	const int guaranteedDamageOnAttacker = (guaranteedDamageRatio * defenderStrength);
	const int guaranteedDamageOnDefender = (guaranteedDamageRatio * attackerStrength);
	attackerStrengthAdjustment += guaranteedDamageOnAttacker;
	defenderStrengthAdjustment += guaranteedDamageOnDefender;

	// Apply additional damage if threshold passed.
	if(defenderStrength >= 2)
	{
		attackerStrengthAdjustment += 1;
	}
	if(attackerStrength >= 2)
	{
		defenderStrengthAdjustment += 1;
	}

	// Apply completely random damage
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> randomDist(1, 100);
	const int rngVal = randomDist(rng);
	if(rngVal <= 25)
	{
		defenderStrengthAdjustment += 1;
	}
	else if(rngVal >= 76)
	{
		attackerStrengthAdjustment += 1;
	}
	else if(41 <= rngVal <= 60)
	{
		defenderStrengthAdjustment += 1;
		attackerStrengthAdjustment += 1;
	}

	// Partially randomised damage to each army.
	const double strengthRatio = attackerStrength / defenderStrength;
	const double maxMultiplier = 0.4;
	const double minMultiplier = 0.1;
	const double maxAttacker = maxMultiplier * strengthRatio;
	const double minAttacker = minMultiplier * strengthRatio;
	const double maxDefender = maxMultiplier / strengthRatio;
	const double minDefender = minMultiplier / strengthRatio;
	std::uniform_real_distribution<double> attackerDist(minAttacker, maxAttacker);  // Percent of strength defending army loses.
	std::uniform_real_distribution<double> defenderDist(minDefender, maxDefender);  // Percent of strength attacking army loses.
	defenderStrengthAdjustment += std::round(attackerDist(rng) * defenderTrueStrength);
	attackerStrengthAdjustment += std::round(defenderDist(rng) * attackerStrength);

	// Ensure some strength adjustment takes place.
	if(defenderStrengthAdjustment == 0 && attackerStrengthAdjustment == 0)
	{
		++defenderStrengthAdjustment;
		++attackerStrengthAdjustment;
	}

	// Reduce strength of both navies.
	assert(defenderStrengthAdjustment >= 0);
	assert(attackerStrengthAdjustment >= 0);
	defendingNavy.reduceStrength(defenderStrengthAdjustment);
	reduceStrength(attackerStrengthAdjustment);
}

int NavalFleet::getNavalMoveStaminaCost()
{
	return 1;
}

int NavalFleet::getAttackStaminaCost()
{
	return 1;
}

std::pair<int, int> NavalFleet::calculateMinMaxStaminaCost(const Territory &territory) const
{
	return { 1, 1 };
}



