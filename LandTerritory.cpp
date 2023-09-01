#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, Grid graphics)
	: Territory(id, graphics)
{
}

LandTerritory::LandTerritory(int id)
	: Territory(id, createRandomLandColor())
{
}

void LandTerritory::update(Message message)
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

bool LandTerritory::occupy(LandArmy *army)
{
	bool isValid = false;

	if(this->army == nullptr)
	{
		this->army = army;
		army->setTerritory(this);
		isValid = true;
		// Observer methods.
		this->army->addObserver(this);  
		notifyObservers(newOccupant);
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
		army->attack(*this->army, getDefenceMultiplier());
		// Attacking army occupys land if defending army killed.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->army == nullptr && !army->isDead())
		{
			this->army = army;
			army->setTerritory(this);
			isValid = true;
			// Observer methods.
			this->army->addObserver(this);
			notifyObservers(newOccupant);
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

bool LandTerritory::occupy(NavalFleet *fleet)
{
	return false;  
}

void LandTerritory::setIsCoastal(bool isCoastal)
{
	this->isCoastal = isCoastal;
}

bool LandTerritory::getIsCoastal() const
{
	return isCoastal;
}

Player* LandTerritory::getOccupant()
{
	if(army != nullptr)
	{
		return &army->getOwner();
	}
	return nullptr;
}

const LandArmy* LandTerritory::getArmy() const
{
	return army;
}

const NavalFleet* LandTerritory::getFleet() const
{
	assert(!isCoastal || fleet == nullptr);
	return fleet;
}

std::string LandTerritory::getSaveLabel() const
{
	return landSaveLabel;
}

void LandTerritory::updateMilitaryPosition()
{
	if(army != nullptr)
	{
		army->setSpritePosition(getCenter());
	}
}
