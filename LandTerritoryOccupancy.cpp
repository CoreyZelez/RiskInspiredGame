#include "LandTerritoryOccupancy.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "LandTerritory.h"
#include <assert.h>

LandTerritoryOccupancy::LandTerritoryOccupancy(LandTerritory &territory)
	: territory(territory)
{
}

void LandTerritoryOccupancy::update(Message message)
{
	switch(message)
	{
	case deadMilitary:
		if(army != nullptr && army->getStrength() == 0)
		{
			army = nullptr;
		}
		if(fleet != nullptr && fleet->getStrength() == 0)
		{
			fleet = nullptr;
		}
	}
}

bool LandTerritoryOccupancy::occupy(LandArmy *army)
{
	bool isValid = false;

	if(this->army == nullptr)
	{
		this->army = army;
		army->setTerritory(&territory);
		isValid = true;
		// Observer methods.
		this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
		territory.notifyObservers(newOccupant);
	}
	// Case armies have same owner.
	else if(&(army->getOwner()) == &(this->army->getOwner()))
	{
		const int initialStrengthSum = army->getStrength() + this->army->getStrength();

		// Absorb strength of army into this->army.
		const int armyStrength = army->getStrength();
		assert(armyStrength > 0);
		this->army->adjustStrength(army->getStrength());
		army->adjustStrength(-army->getStrength());  // Sets strength to 0.

		const int finalStrengthSum = army->getStrength() + this->army->getStrength();
		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.

		isValid = true;
	}
	// Case armies have different owner.
	else
	{
		// Temporarily implemented as always attack. In future potentially not as there may be friendly sharing of troop territory.
		army->attack(*this->army, defenceMultiplier);
		// Attacking army occupys land if defending army killed.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->army == nullptr && !army->isDead())
		{
			this->army = army;
			army->setTerritory(&territory);
			isValid = true;
			// Observer methods.
			this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
			territory.notifyObservers(newOccupant);
		}
	}

	// Remove pointer to army if army is dead.
	if(army != nullptr && army->getStrength() == 0)
	{
		army = nullptr;
	}

	// Update the positions of military unit sprites.
	updateMilitaryPosition();

	return isValid;  // WARNING CURRENTLY USELESS. IS THIS EVER NEEDED!!!
}

bool LandTerritoryOccupancy::occupy(NavalFleet *fleet)
{
	return false;
}

Player* LandTerritoryOccupancy::getOccupant()
{
	if(army != nullptr)
	{
		return &army->getOwner();
	}
	return nullptr;
}

const LandArmy* LandTerritoryOccupancy::getArmy() const
{
	return army;
}

const NavalFleet * LandTerritoryOccupancy::getFleet() const
{
	return fleet;
}

void LandTerritoryOccupancy::updateMilitaryPosition()
{
	if(army != nullptr)
	{
		army->setSpritePosition(territory.getGrid().getCenter());
	}
}
