#include "Barony.h"
#include "LandTerritory.h"
#include <assert.h>

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

Barony::Barony(Player* ruler, LandTerritory &territory, double landArmyYield)
	: LandedEstate(Title::baron, ruler, territory), 
	landArmyYield(landArmyYield)
{
}

Barony::Barony(Player *ruler, LandTerritory &territory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::baron, ruler, territory),
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield), canYieldNavy(true)
{
}

std::shared_ptr<LandArmy> Barony::yieldLandArmy()
{
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
	const int threshold = 2;  // Min cumulative value for yield to take place.
	if(canYieldNavy && cumulativeNavalFleet >= threshold)
	{
		const int yield = cumulativeNavalFleet;
		cumulativeNavalFleet = 0;
		return putFleet(yield);
	}

	return nullptr;
}

