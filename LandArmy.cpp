#include "LandArmy.h"
#include "Territory.h"
#include "TextureManager.h"
#include "Player.h"
#include <assert.h>
#include <random>
#include <iostream>

LandArmy::LandArmy(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, *(TextureManager::getInstance().getTexture("landArmy")))
{
	assert(location != nullptr);
}

void LandArmy::attack(LandArmy &defendingArmy, double defenceMultiplier)
{
	const double defenderStrength = static_cast<double>(defendingArmy.getStrength());
	const double defenderAdjustedStrength = defenderStrength * defenceMultiplier;  // defender strength.
	const double attackerStrength = getStrength();
	assert(defenderStrength > 0);
	assert(attackerStrength > 0);

	// Adjust strengths for passing a given threshold.
	const double strengthThreshold = 3;  // Threshold for guaranteed damage on opponent.
	const int thresholdAdjustmentValue = -1;  // Strength adjustment for opponent passing threshold.
	if(defenderAdjustedStrength >= strengthThreshold)
	{
		adjustStrength(thresholdAdjustmentValue);
	}
	if(attackerStrength >= strengthThreshold)
	{
		defendingArmy.adjustStrength(thresholdAdjustmentValue);
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
	int defenderStrengthAdjustment = -std::round(attackerDist(rng) * defenderStrength);
	int attackerStrengthAdjustment = -std::round(defenderDist(rng) * attackerStrength);
	assert(defenderStrengthAdjustment <= 0);
	assert(attackerStrengthAdjustment <= 0);
	defendingArmy.adjustStrength(defenderStrengthAdjustment);
	adjustStrength(attackerStrengthAdjustment);

	// Special case where no damage is taken by either army.
	if(getStrength() == attackerStrength && defendingArmy.getStrength() == defenderStrength)
	{
		assert(attackerStrength < strengthThreshold && defenderStrength < strengthThreshold);
		assert(attackerStrengthAdjustment == 0 && defenderStrengthAdjustment == 0);

		// Determine strength adjustment.
		std::uniform_int_distribution<int> dist(0, 1);
		if(defenderAdjustedStrength > attackerStrength)
		{
			--attackerStrengthAdjustment;
		}
		else if(defenderAdjustedStrength < attackerStrength)
		{
			--defenderStrengthAdjustment;
		}
		else if(dist(rng) == 1)
		{
			--attackerStrengthAdjustment;
		}
		else
		{
			--defenderStrengthAdjustment;
		}

		// Adjust army strengths.
		defendingArmy.adjustStrength(defenderStrengthAdjustment);
		adjustStrength(attackerStrengthAdjustment);
	}
}

void LandArmy::move(Territory &territory, int strength)
{
	// Attempt to move to current location.
	if(&territory == &getTerritory())
	{
		return;
	}

	assert(strength > 0);
	assert(strength <= getStrength());

	// Amount to adjust military strength. 
	int strengthAdjustment = -strength;

	/// IN FUTURE USE FACTORY TO CREATE ARMY!!! SHOULD AUTOMATICALLY STORE ARMY IN PLAYER MILITARYMANAGER!!!
	std::unique_ptr<LandArmy> deployedArmy = std::make_unique<LandArmy>(getOwner(), &getTerritory(), strength);  // Land army attempting location occupation.

	// Attempt occupation of location by deployed army.
	territory.occupy(deployedArmy.get());

	// Refund strength to this->army if deployedArmy is not able to occupy location
	if(&deployedArmy.get()->getTerritory() == &getTerritory())
	{
		const int strengthRefund = deployedArmy.get()->getStrength();
		assert(strengthRefund >= 0);
		strengthAdjustment += strengthRefund;
		deployedArmy.reset();
	}
	// Add the army to the military manager of owning player.
	else
	{
		getOwner().getMilitaryManager().addLandArmy(std::move(deployedArmy));
	}

	// Adjust strength of this army.
	adjustStrength(strengthAdjustment);
}


