#include "Barony.h"
#include "LandTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Barony::Barony(LandTerritory &landTerritory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::baron, landTerritory),
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield), landTerritory(landTerritory)
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

std::unique_ptr<LandArmy> Barony::yieldLandArmy()
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

std::unique_ptr<NavalFleet> Barony::yieldNavalFleet()
{
	// Add navy per turn yield to cumulative yield.
	cumulativeNavalFleet += navalFleetYield;

	// Yield navy to territory and player if threshold surpassed.
	// Only yields navy if territory has a port.
	const int navalFleetThreshold = 2;  // Min cumulative value for yield to take place.
	if(landTerritory.hasPort() && cumulativeNavalFleet >= navalFleetThreshold)
	{
		const int yield = cumulativeNavalFleet;
		cumulativeNavalFleet -= yield;
		return putFleet(yield);
	}

	return nullptr;
}

std::unique_ptr<NavalFleet> Barony::putFleet(int strength)
{
	// Should not be hostile army residing on this territory.
	// There may however be a hostile army on the naval territory associated with the port.
	assert(territory.getOccupancyHandler()->getOccupant() == nullptr
		|| territory.getOccupancyHandler()->getOccupant() == getRuler());

	// Estate does not generate naval units if no port.
	if(landTerritory.getPort() == nullptr)
	{
		return nullptr;
	}

	// Territory we place the fleet on.
	NavalTerritory &navalTerritory = landTerritory.getPort().get()->getNavalTerritory();

	std::unique_ptr<NavalFleet> fleet = std::make_unique<NavalFleet>(*getRuler(), &navalTerritory, strength);
	navalTerritory.getOccupancyHandler()->occupy(fleet.get());

	// Fleet merged with pre-existing fleet on territory or was killed by it.
	if(fleet->isDead())
	{
		return nullptr;
	}

	// There was no pre-existing fleet on territory.
	assert(fleet.get()->getTotalStrength() > 0);
	return fleet;
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

