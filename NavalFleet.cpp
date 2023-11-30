#include "NavalFleet.h"
#include "Territory.h"
#include "Player.h"
#include "TextureManager.h"
#include <assert.h>
#include <memory>
#include <random>

NavalFleet::NavalFleet(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, *(TextureManager::getInstance().getTexture("landArmy")))
{
	assert(location != nullptr);
}

NavalFleet::NavalFleet(Player & owner, Territory * location, std::array<unsigned int, 4> staminaStrength)
	: MilitaryForce(owner, location, staminaStrength, *(TextureManager::getInstance().getTexture("landArmy")))
{
	assert(location != nullptr);
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
	std::array<unsigned int, 4> expendedStrength = expendStrength(strength, location);
	// Naval fleet attempting location occupation.
	std::unique_ptr<NavalFleet> newFleet = std::make_unique<NavalFleet>(getOwner(), &getTerritory(), expendedStrength);
	// Only proceed if new army strength is greater than 0.
	if(newFleet.get()->isDead())
	{
		return;
	}

	// Attempt occupation of location by new fleet.
	location.getOccupancyHandler()->occupy(newFleet.get());

	// Refund strength to this->fleet if deployed fleet is not able to occupy location
	if(&newFleet.get()->getTerritory() == &getTerritory())
	{
		std::array<unsigned int, 4> strengthRefund = newFleet.get()->getStaminaStrength();
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

void NavalFleet::moveClosest(Territory & target, unsigned int strength, int maxDist)
{
}

void NavalFleet::attack(NavalFleet & defendingNavy, double defenceMultiplier)
{
	const double defenderStrength = static_cast<double>(defendingNavy.getTotalStrength());
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
	defendingNavy.reduceStrength(defenderStrengthAdjustment);
	reduceStrength(attackerStrengthAdjustment);

	// Special case where no damage is taken by either army.
	if(getTotalStrength() == attackerStrength && defendingNavy.getTotalStrength() == defenderStrength)
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

		// Adjust navy strengths.
		defendingNavy.reduceStrength(defenderStrengthAdjustment);
		reduceStrength(attackerStrengthAdjustment);
	}
}

std::pair<int, int> NavalFleet::calculateMinMaxStaminaCost(const Territory & territory) const
{
	// temp.
	return { 4, 4 };
}

