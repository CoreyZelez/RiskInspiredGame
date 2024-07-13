#include "LandTerritoryOccupancy.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "LandTerritory.h"
#include "Player.h"
#include "GameplaySettings.h"
#include <assert.h>
#include <iostream>

LandTerritoryOccupancy::LandTerritoryOccupancy(LandTerritory &territory)
	: territory(territory)
{
}

void LandTerritoryOccupancy::update(Message message)
{
	switch(message)
	{
	case vacatedMilitary:
		if(army != nullptr && army->isDead())
		{
			army = nullptr;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void LandTerritoryOccupancy::determineOccupation()
//{
//	if(army != nullptr)
//	{
//		const Player& armyOwner = army->getOwner();
//		if(!sameUpperRealm(controller, &armyOwner))
//		{
//			territory.notifyObservers(newController);
//		}
//	}
//}

void LandTerritoryOccupancy::occupy(LandArmy *army)
{
	bool isValid = false;

	// Case no pre existing occupying army on terriory
	if(this->army == nullptr)
	{
		changeOccupyingArmy(army);
	}
	// Case armies have same owner.
	else if(&(army->getOwner()) == &(this->army->getOwner()))
	{
		const int initialStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();

		// Absorb strength of army into this->army.
		assert(army->getTotalStrength() > 0);
		this->army->increaseStrength(army->getStaminaStrength());
		army->clearStrength();  // Sets strength to 0.

		const int finalStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();
		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.
	}
	// Case armies have different owner.
	else
	{
		army->attack(*this->army, territory.getTerrain());

		// Attacking army occupys land if defending army killed.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->army == nullptr && !army->isDead())
		{
			changeOccupyingArmy(army);
		}
	}

	// Remove pointer to army if army is dead.
	if(this->army != nullptr && this->army->isDead())
	{
		this->army = nullptr;
	}

	// Update the positions of military unit sprites.
	updateMilitaryPosition();
}

void LandTerritoryOccupancy::occupy(NavalFleet *fleet)
{
	// Intentionally empty.
}

void LandTerritoryOccupancy::forceOccupy(LandArmy *army)
{
	// Attempt occupancy.
	territory.getOccupancyHandler()->occupy(army);
	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!army->isDead() && controller != &army->getOwner())
	{
		occupy(army);
	}
}

void LandTerritoryOccupancy::forceOccupy(NavalFleet *fleet)
{
	assert(false);
}

void LandTerritoryOccupancy::transferControl(Player& player)
{
	assert(!player.hasLiege());
	assert(army == nullptr || controller == &army->getOwner());

	controller = &player;
	territory.notifyObservers(newController);

	// Change the controller back if army present.
	if(army != nullptr)
	{
		controller = &army->getOwner();
		territory.notifyObservers(newController);
	}
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

void LandTerritoryOccupancy::changeOccupyingArmy(LandArmy *army)
{
	assert(this->army == nullptr);
	assert(getController() == nullptr || !getController()->hasLiege());

	this->army = army;
	army->setTerritory(&territory);

	if(controller != &army->getOwner())
	{
		controller = &army->getOwner();
		territory.notifyObservers(newController);
	}

	// Observer methods.
	this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
}