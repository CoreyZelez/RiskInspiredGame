#include "LiegePolicy.h"
#include "Player.h"

LiegePolicy::LiegePolicy(const Player &player)
	: player(player)
{
}

void LiegePolicy::update()
{
	if(resistanceThreshold > baseResistanceThreshold)
	{
		--resistanceThreshold;
	}
}

void LiegePolicy::adjustResistanceThreshold(const Player &rebellingVassal)
{
	const int vassalStrength = rebellingVassal.getMilitaryManager().getTotalArmyStrength(false);
	const int liegeStrength = player.getMilitaryManager().getTotalArmyStrength(false);
	const double strengthRatio = (double)vassalStrength / (double)liegeStrength;
	const double adjustedRatio = (4 * strengthRatio) + 0.5;
	const int resistanceThresholdIncrement = adjustedRatio * 10;
	resistanceThreshold += resistanceThresholdIncrement;
	if(resistanceThreshold > maximumResistanceThreshold)
	{
		resistanceThreshold = maximumResistanceThreshold;
	}
}

