#include "VassalPolicy.h"
#include "LiegePolicy.h"
#include "Player.h"
#include <iostream>

VassalPolicy::VassalPolicy(const Player & player)
	: player(player)
{
}

void VassalPolicy::handleLiegeInfluenceChange(const LiegePolicy &liegePolicy)
{
	const double reductionConstant = 0.3;
	liegeInfluence += (liegeLevyContribution - liegePolicy.vassalLevy) / (liegePolicy.vassalLevy + reductionConstant);
	const double reductionFactor = 0.99;
	liegeInfluence *= reductionFactor;
}
 
void VassalPolicy::handleResistanceChange(const Player &liege, const LiegePolicy &liegePolicy)
{
	// Maximum resistance that can be attained.
	const double maxResistance = 2 * rebellionThreshold;

	// Resistance reduces by multiplication factor.
	// This stops resistance from growing far past the rebellion threshold. 
	const double resistanceMultiplier = 0.995;
	resistance *= resistanceMultiplier;

	// Increase resistance by some constant.
	const double resistanceConstant = 0.3;
	resistance += resistanceConstant;

	// Adjust resistance due to liege levy.
	const double equilibriumLevy = 0.4;
	const double residualLevy = liegePolicy.vassalLevy - equilibriumLevy;
	const double levyResistance = 2 * residualLevy;
	resistance += levyResistance;

	// Adjust resistance due to difference between liege and players highest title.
	const Title highestLiegeTitle = liege.getRealm().getHighestRulerTitle();
	const Title highestPlayerTitle = player.getRealm().getHighestRulerTitle();
	// + 1 since having same highest title should increase resistance.
	const int titleDifference = (int)highestPlayerTitle - (int)highestLiegeTitle + 1;
	double titleResistance = titleDifference * 0.1;
	// Relatively more change to resistance when player's title exceeds liege's. 
	if(titleResistance > 0)
	{
		titleResistance *= 2;
	}
	resistance += titleResistance;

	// Adjust resistance due to liege not having dejure upper title of player's highest title(s).

	// Adjust resistance due to total military strength relative to liege's.
	const double equilibriumRatio = 2;
	const int totalLiegeArmyStrength = liege.getMilitaryManager().getTotalArmyStrength();
	const int totalVassalArmyReserves = liege.getRealm().getTotalVassalArmyReserves();
	if(totalLiegeArmyStrength == 0)
	{
		resistance = maxResistance;
	}
	else
	{
		const double armyRatio = totalVassalArmyReserves / totalLiegeArmyStrength;
		const double residualRatio = armyRatio - equilibriumRatio;
		const double armyResistance = residualRatio * 0.1;
		resistance += armyResistance;
	}

	// Resistance is non negative.
	if(resistance < 0)
	{
		resistance = 0;
	}
	// Resistance cannot surpass maximum.
	else if(resistance > maxResistance)
	{
		resistance = maxResistance;
	}
}
