#include "Barony.h"
#include "LandTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "Player.h"
#include "Realm.h"
#include "LandTerritoryOccupancy.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Barony::Barony(const GameplaySettings &gameplaySettings, LandTerritory& landTerritory, sf::Color color)
	: LandedEstate(Title::barony, landTerritory, color), siegeManager(gameplaySettings, *this)
{
	dynamic_cast<LandTerritoryOccupancy*>(territory.getOccupancyHandler())->initSiegeManager(siegeManager);
}

void Barony::update()
{
	siegeManager.update();
}

void Barony::updateSiege()
{
	siegeManager.updateSiege();
}

void Barony::setOwnership(Player* ruler, bool recurseOnParents)
{
	assert(!ruler->hasLiege());

	LandedEstate::setOwnership(ruler, recurseOnParents);
	// Determine controller since may change if territory unoccupied.
	territory.getOccupancyHandler()->determineController();
}

std::unique_ptr<LandArmy> Barony::yieldLandArmy()
{
	assert(siegeManager.activeSiege() || sameUpperRealm(getTerritory().getController(), getRuler()));

	if(siegeManager.activeSiege())
	{
		return nullptr;
	}

	const double landArmyYield = territory.getFeatures().calculateArmyYield();

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
	if(!territory.hasPort() || siegeManager.activeSiege())
	{
		return nullptr;
	}

	const double navalFleetYield = territory.getFeatures().calculateFleetYield();

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

	// Territory must be controlled by player putting fleet.
	assert(getTerritory().getController() == nullptr || getTerritory().getController() == getRuler());
	assert(getTerritory().getOccupancyHandler()->getArmy() == nullptr || &getTerritory().getOccupancyHandler()->getArmy()->getOwner() == getRuler());


	// Estate does not generate naval units if no port.
	if(territory.getPort() == nullptr)
	{
		return nullptr;
	}

	// Territory we place the fleet on.
	NavalTerritory &navalTerritory = territory.getPort().get()->getNavalTerritory();

	// Yield fleet.
	Player &player = *getRuler();
	std::unique_ptr<NavalFleet> fleet = std::make_unique<NavalFleet>(player, &navalTerritory, strength);

	// Attempt occupancy.
	navalTerritory.getOccupancyHandler()->occupy(fleet.get());
	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!fleet.get()->isDead() && navalTerritory.getController() != &player)
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
	const double landArmyYield = territory.getFeatures().calculateArmyYield();
	const double navalFleetYield = territory.getFeatures().calculateFleetYield();

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

