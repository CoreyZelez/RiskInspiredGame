#include "Barony.h"
#include "LandTerritory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Barony::Barony(LandTerritory &territory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::baron, territory),
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield), yieldsNavy(territory.getIsCoastal())
{

}

void Barony::saveToFile(std::ofstream & file) const
{
	LandedEstate::saveToFile(file);
	file << "# land army yield" << std::endl;
	file << landArmyYield << std::endl;
	file << "# naval fleet yield" << std::endl;
	file << navalFleetYield << std::endl;
}

std::shared_ptr<LandArmy> Barony::yieldLandArmy()
{
	// Add army per turn yield to cumulative yield.
	cumulativeLandArmy += landArmyYield;

	// Yield army to territory and player if threshold surpassed.
	const int landArmyThreshold = 3;  // Min cumulative value for yield to take place.
	if(cumulativeLandArmy >= landArmyThreshold)
	{
		const int yield = cumulativeLandArmy;
		cumulativeLandArmy -= yield;
		return putArmy(yield);
	}
	return nullptr;
}

std::shared_ptr<NavalFleet> Barony::yieldNavalFleet()
{
	// Add navy per turn yield to cumulative yield.
	cumulativeNavalFleet += navalFleetYield;

	// Yield navy to territory and player if threshold surpassed.
	const int navalFleetThreshold = 2;  // Min cumulative value for yield to take place.
	if(yieldsNavy && cumulativeNavalFleet >= navalFleetThreshold)
	{
		const int yield = cumulativeNavalFleet;
		cumulativeNavalFleet -= yield;
		return putFleet(yield);
	}

	return nullptr;
}

void Barony::receiveBonusYield(const float &bonus)
{
	cumulativeLandArmy += landArmyYield * bonus;
	cumulativeNavalFleet += navalFleetYield * bonus;
}

std::string Barony::getSaveLabel() const
{
	return baronySaveLabel;
}

