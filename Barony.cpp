#include "Barony.h"
#include "LandTerritory.h"
#include <assert.h>
#include <iostream>

Barony::Barony(LandTerritory &territory, double landArmyYield)
	: LandedEstate(Title::baron, territory),
	landArmyYield(landArmyYield)
{
}

Barony::Barony(LandTerritory &territory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::baron, territory),
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield), canYieldNavy(true)
{
}

std::shared_ptr<LandArmy> Barony::yieldLandArmy()
{
	// Add army per turn yield to cumulative yield.
	cumulativeLandArmy += landArmyYield;

	// Yield army to territory and player if threshold surpassed.
	const int threshold = 3;  // Min cumulative value for yield to take place.
	if(cumulativeLandArmy >= threshold)
	{
		const int yield = cumulativeLandArmy;
		cumulativeLandArmy = 0;
		return putArmy(yield);
	}
	return nullptr;
}

std::shared_ptr<NavalFleet> Barony::yieldNavalFleet()
{
	// Add navy per turn yield to cumulative yield.
	cumulativeNavalFleet += navalFleetYield;

	// Yield navy to territory and player if threshold surpassed.
	const int threshold = 2;  // Min cumulative value for yield to take place.
	if(canYieldNavy && cumulativeNavalFleet >= threshold)
	{
		const int yield = cumulativeNavalFleet;
		cumulativeNavalFleet = 0;
		return putFleet(yield);
	}

	return nullptr;
}

void Barony::receiveBonusYield(const float &bonus)
{
	cumulativeLandArmy += landArmyYield * bonus;
	cumulativeNavalFleet += navalFleetYield * bonus;
}

