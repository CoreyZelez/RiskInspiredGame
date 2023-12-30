#include "Barony.h"
#include "LandTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "Player.h"
#include "Realm.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Barony::Barony(LandTerritory &landTerritory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::barony, landTerritory),
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield), landTerritory(landTerritory)
{
}

void Barony::saveToFile(std::ofstream &file) const
{
	LandedEstate::saveToFile(file);
	file << "# land army yield" << std::endl;
	file << landArmyYield << std::endl;
	file << "# naval fleet yield" << std::endl;
	file << navalFleetYield << std::endl;
}

std::unique_ptr<LandArmy> Barony::yieldLandArmy()
{
	if(!getRuler()->getRealm().hasLiege())
	{
		// Percent of yielded army allocated to reinforcement.
		const float armyReinforcementRate = getRuler()->getMilitaryManager().getArmyReinforcementRate();

		double armyReinforcementYield = armyReinforcementRate * landArmyYield;
		double armyLocalYield = landArmyYield - armyReinforcementYield;
		assert(armyLocalYield >= 0);
		assert(armyReinforcementYield >= 0);

		// Adds reinforcements to rulers military manager.
		getRuler()->getMilitaryManager().addArmyReinforcements(armyReinforcementYield);
		// Adds local army yield to cumulative yield of this barony.
		cumulativeLandArmy += armyLocalYield;

		// Yield army to territory and player if threshold surpassed.
		const int landArmyThreshold = 3;  // Min cumulative value for yield to take place.
		if(cumulativeLandArmy >= landArmyThreshold)
		{
			const int yield = cumulativeLandArmy;
			cumulativeLandArmy -= yield;
			return putArmy(yield);
		}
	}
	else
	{
		// Yield entirely to reserves since ruler is a vassal.
		getRuler()->getMilitaryManager().addArmyReserves(landArmyYield);
	}

	return nullptr;
}

std::unique_ptr<NavalFleet> Barony::yieldNavalFleet()
{
	if(!getRuler()->getRealm().hasLiege())
	{
		if(!landTerritory.hasPort())
		{
			return nullptr;
		}

		// Add navy per turn yield to cumulative yield.
		cumulativeNavalFleet += navalFleetYield;

		// Yield navy to territory and player if threshold surpassed.
		// Only yields navy if territory has a port.
		const int navalFleetThreshold = 2;  // Min cumulative value for yield to take place.
		if(cumulativeNavalFleet >= navalFleetThreshold)
		{
			const int yield = cumulativeNavalFleet;
			cumulativeNavalFleet -= yield;
			return putFleet(yield);
		}

		return nullptr;
	}
	else
	{
		// Yield entirely to reserves since ruler is a vassal.
		getRuler()->getMilitaryManager().addFleetReserves(navalFleetYield);
	}
}

std::unique_ptr<NavalFleet> Barony::putFleet(int strength)
{
	assert(getRuler() != nullptr);

	// Should not be hostile army residing on this territory.
	// There may however be a hostile army on the naval territory associated with the port.
	assert(getTerritory().getOccupancyHandler()->getOccupant() == nullptr
		|| getTerritory().getOccupancyHandler()->getOccupant() == getRuler());

	// Estate does not generate naval units if no port.
	if(landTerritory.getPort() == nullptr)
	{
		return nullptr;
	}

	// Territory we place the fleet on.
	NavalTerritory &navalTerritory = landTerritory.getPort().get()->getNavalTerritory();

	std::unique_ptr<NavalFleet> fleet = std::make_unique<NavalFleet>(*getRuler(), &navalTerritory, strength);
	// Attempt occupancy.
	navalTerritory.getOccupancyHandler()->occupy(fleet.get());
	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!fleet.get()->isDead() && navalTerritory.getOccupancyHandler()->getOccupant() != getRuler())
	{
		navalTerritory.getOccupancyHandler()->occupy(fleet.get());
	}

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
	if(!getRuler()->getRealm().hasLiege())
	{
		// Yield all land army units as reinforcements.
		const float armyReinforcements = landArmyYield * bonus;
		getRuler()->getMilitaryManager().addArmyReinforcements(armyReinforcements);

		// Yield all naval fleets to this barony.
		const float fleetYield = navalFleetYield * bonus;
		cumulativeNavalFleet += fleetYield;
	}
	else
	{
		// Yield all land army units to reserves since ruler is a vassal.
		const float armyReserves = landArmyYield * bonus;
		getRuler()->getMilitaryManager().addArmyReserves(armyReserves);
		// Yield all naval fleet units to reserves since ruler is a vassal.
		const float fleetReserves = navalFleetYield * bonus;
		getRuler()->getMilitaryManager().addFleetReserves(fleetReserves);
	}
}

std::string Barony::getSaveLabel() const
{
	return baronySaveLabel;
}

