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

Barony::Barony(LandTerritory &landTerritory)
	: LandedEstate(Title::barony, landTerritory),
	landTerritory(landTerritory)
{
}

std::unique_ptr<LandArmy> Barony::yieldLandArmy()
{
	const double landArmyYield = landTerritory.getFeatures().calculateArmyYield();

	if(!getRuler()->hasLiege())
	{
		// Percent of yielded army allocated to reinforcement.
		const float armyReinforcementRate = getRuler()->getMilitaryManager().getArmyReinforcementRate();
		// Base reinforcements yield.
		double armyReinforcementYield = armyReinforcementRate * landArmyYield;
		assert(armyReinforcementYield >= 0);

		// Base local yield to territory associated with barony.
		const double armyLocalYield = (landArmyYield - armyReinforcementYield);
		// Adjust local yield for army yield ratio.
		const double armyYieldMultiplier = getRuler()->getRealm().getEffectiveArmyYieldRatio();
		const double armyLocalAdjustedYield = armyLocalYield * armyYieldMultiplier;
		assert(armyLocalAdjustedYield >= 0);

		// Adds reinforcements to rulers military manager. 
		// Note we provide the base reinforcement yield. The function adjusts for this using the army reinforcement rate as
		// we did for the local army adjusted yield.
		getRuler()->handleReinforcementArmyYield(armyReinforcementYield);

		// Adds local army yield to cumulative yield of this barony.
		cumulativeLandArmy += armyLocalAdjustedYield;

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
		// Yield to reserves and provide levies to liege. 
		getRuler()->handleReserveArmyYield(landArmyYield);
	}

	return nullptr;
}

std::unique_ptr<NavalFleet> Barony::yieldNavalFleet()
{	
	if(!landTerritory.hasPort())
	{
		return nullptr;
	}

	const double navalFleetYield = landTerritory.getFeatures().calculateFleetYield();

	if(!getRuler()->hasLiege())
	{
		// Percent of yielded fleet allocated to reinforcement.
		const float fleetReinforcementRate = getRuler()->getMilitaryManager().getFleetReinforcementRate();
		// Base reinforcements yield.
		const double fleetReinforcementYield = fleetReinforcementRate * navalFleetYield;
		assert(fleetReinforcementYield >= 0);

		// Base local yield to territory associated with barony.
		const double fleetLocalYield = (navalFleetYield - fleetReinforcementYield);
		// Adjust local yield for fleet yield ratio.
		const double fleetYieldMultiplier = getRuler()->getRealm().getEffectiveFleetYieldRatio();
		const double fleetLocalAdjustedYield = fleetLocalYield * fleetYieldMultiplier;
		assert(fleetLocalAdjustedYield >= 0);

		// Adds reinforcements to rulers military manager. 
		// Note we provide the base reinforcement yield. The function adjusts for this using the fleet reinforcement rate as
		// we did for the local fleet adjusted yield.
		getRuler()->handleReinforcementFleetYield(fleetReinforcementYield);

		// Adds local fleet yield to cumulative yield of this barony.
		cumulativeNavalFleet += fleetLocalAdjustedYield;

		// Yield fleet to territory and player if threshold surpassed.
		const int navalFleetThreshold = 3;  // Min cumulative value for yield to take place.
		if(cumulativeNavalFleet >= navalFleetThreshold)
		{
			const int yield = cumulativeNavalFleet;
			cumulativeNavalFleet -= yield;
			return putFleet(yield);
		}
	}
	else
	{
		// Yield to reserves and provide levies to liege. 
		getRuler()->handleReserveFleetYield(navalFleetYield);
	}

	return nullptr;
}

std::unique_ptr<NavalFleet> Barony::putFleet(int strength)
{
	assert(getRuler() != nullptr); 
	assert(!getRuler()->hasLiege());  // Ruler should not have a liege.

	// Should not be hostile army residing on this territory.
	// There may however be a hostile army on the naval territory associated with the port.
	assert(getTerritory().getOccupancyHandler()->getOccupant() == nullptr
		|| sameUpperRealm(getRuler(), getTerritory().getOccupancyHandler()->getOccupant()));

	// Estate does not generate naval units if no port.
	if(landTerritory.getPort() == nullptr)
	{
		return nullptr;
	}

	// Territory we place the fleet on.
	NavalTerritory &navalTerritory = landTerritory.getPort().get()->getNavalTerritory();

	// Yield fleet.
	Player &player = *getRuler();
	std::unique_ptr<NavalFleet> fleet = std::make_unique<NavalFleet>(player, &navalTerritory, strength);

	// Attempt occupancy.
	navalTerritory.getOccupancyHandler()->occupy(fleet.get());
	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!fleet.get()->isDead() && !sameUpperRealm(navalTerritory.getOccupancyHandler()->getOccupant(), &player))
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
	const double landArmyYield = landTerritory.getFeatures().calculateArmyYield();
	const double navalFleetYield = landTerritory.getFeatures().calculateFleetYield();

	if(!getRuler()->hasLiege())
	{
		// Yield all land army units as reinforcements.
		const float armyReinforcements = landArmyYield * bonus;
		getRuler()->handleReinforcementArmyYield(armyReinforcements);

		// Yield all naval fleets to this barony.
		const float fleetYield = navalFleetYield * bonus;
		cumulativeNavalFleet += fleetYield;
	}
	else
	{
		// Yield army to reserves and provide levies to liege. 
		const float armyReserves = landArmyYield * bonus;
		getRuler()->handleReserveArmyYield(landArmyYield);

		// Yield all fleets to reserves and provide levies to liege..
		const float fleetYield = navalFleetYield * bonus;
		getRuler()->handleReserveFleetYield(fleetYield);
	}
}

std::string Barony::getSaveLabel() const
{
	return baronySaveLabel;
}

