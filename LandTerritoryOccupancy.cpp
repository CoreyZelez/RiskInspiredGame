#include "LandTerritoryOccupancy.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "LandTerritory.h"
#include "Player.h"
#include "GameplaySettings.h"
#include "SiegeManager.h"
#include <assert.h>
#include <iostream>

LandTerritoryOccupancy::LandTerritoryOccupancy(LandTerritory &territory)
	: territory(territory)
{
}

void LandTerritoryOccupancy::initSiegeManager(SiegeManager& siegeManager)
{
	this->siegeManager = &siegeManager;
}

void LandTerritoryOccupancy::update(Message message)
{
	switch(message)
	{
	case vacatedMilitary:
		if(army != nullptr && army->isDead())
		{
			determineController();
			siegeManager->handleArmyVacated();
			army = nullptr;
		}
	}
}

void LandTerritoryOccupancy::occupy(LandArmy *army)
{
	assert(controller->getRealm().getTerritories().testInvariants());

	bool isValid = false;

	// Case no pre existing occupying army on territory
	if(this->army == nullptr)
	{
		changeOccupyingArmy(*army);
	}
	// Case armies have same owner.
	else if(&(army->getOwner()) == &(this->army->getOwner()))
	{
		assert(army != this->army);
		assert(army->getTotalStrength() > 0);

		const int initialStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();

		this->army->increaseStrength(army->getStaminaStrength());
		army->clearStrength();  // Sets strength to 0.

		const int finalStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();

		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.
	}
	// Case armies have different owner.
	else
	{
		army->attack(*this->army, territory.getTerrain());

		// Attacking army occupies land if defending army killed.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->army == nullptr && !army->isDead())
		{
			changeOccupyingArmy(*army);
		}
	}

	// Remove pointer to army if army is dead.
	if(this->army != nullptr && this->army->isDead())
	{
		this->army = nullptr;
	}

	// Update the positions of military unit sprites.
	updateMilitaryPosition();

	assert(controller->getRealm().getTerritories().testInvariants());
}

void LandTerritoryOccupancy::occupy(NavalFleet *fleet)
{
	// Intentionally empty.
}

void LandTerritoryOccupancy::forceOccupy(LandArmy *army)
{
	// Attempt occupancy.
	territory.getOccupancyHandler()->occupy(army);
	// Repeatedly reattempt occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!army->isDead() && !sameUpperRealm(controller, &army->getOwner()))
	{
		occupy(army);
	}
}

void LandTerritoryOccupancy::forceOccupy(NavalFleet *fleet)
{
	assert(false);
}

const LandArmy* LandTerritoryOccupancy::getArmy() const
{
	return army;
}

LandArmy* LandTerritoryOccupancy::getArmy()
{
	return army;
}

const NavalFleet * LandTerritoryOccupancy::getFleet() const
{
	return nullptr;
}

void LandTerritoryOccupancy::updateMilitaryPosition()
{
	if(army != nullptr)
	{
		army->setSpritePosition(territory.getGrid().getCenter());
	}
}

void LandTerritoryOccupancy::changeOccupyingArmy(LandArmy &army)
{
	assert(this->army == nullptr);

	army.setTerritory(&territory);
	this->army = &army;

	determineController();
	// siegeManager->handleSiegeBegin(army); THIS OCCURS IN DETERMINE CONTROLLER. IS THAT GOOD?

	// Observer methods.
	this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
}

bool LandTerritoryOccupancy::handleControllerChange(Player& player)
{
	// The player the controller is being changed to must either control the army occupying the territory or must be
	// the owner of the barony associated with the territory.
	assert(&siegeManager->getBaronyRuler() == &player || &army->getOwner() == &player);

	if(&player == controller)
	{
		assert(player.getRealm().getTerritories().controlsTerritory(territory));
		return false;
	}
	else 
	{
		if(controller != nullptr)
		{
			controller->getRealm().removeControl(territory);
		}
		player.getRealm().addControl(territory);
		controller = &player;

		return true;
	}

	return false;
}

void LandTerritoryOccupancy::determineController()
{
	if(army != nullptr && !army->isDead())
	{
		if(sameUpperRealm(&siegeManager->getBaronyRuler(), &army->getOwner()))
		{
			handleControllerChange(siegeManager->getBaronyRuler());
		}
		else
		{
			handleControllerChange(army->getOwner());
			siegeManager->handleSiegeBegin(*army);
		}
	}
	else
	{
		handleControllerChange(siegeManager->getBaronyRuler());
	}
}
