#include "NavalTerritoryOccupancy.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include <assert.h>

NavalTerritoryOccupancy::NavalTerritoryOccupancy(NavalTerritory &territory)
	: territory(territory)
{
}

void NavalTerritoryOccupancy::update(Message message)
{
	switch(message)
	{
	case deadMilitary:
		if(army != nullptr && army->isDead())
		{
			army = nullptr;
		}
		if(fleet != nullptr && fleet->isDead())
		{
			fleet = nullptr;
		}
	}
}

bool NavalTerritoryOccupancy::occupy(LandArmy *army)
{
	bool isValid = false;

	// Can successfully occupy with army if army owner is the most recent controller of territory by navy.
	if(this->controller == &army->getOwner())
	{
		if(this->army == nullptr)
		{
			this->army = army;
			army->setTerritory(&territory);
			isValid = true;
			// Observer methods.
			this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
		}
		else
		{
			// Merge armies as there is friendly owned army currently occupying territory.

			// Track initial strength sum for debugging purposes.
			const int initialStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();

			// Absorb strength of army into this->army.
			assert(army->getTotalStrength() > 0);
			this->army->increaseStrength(army->getStaminaStrength());
			army->clearStrength();  // Sets strength to 0.

			const int finalStrengthSum = army->getTotalStrength() + this->army->getTotalStrength();
			assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.

			isValid = true;
		}
	}

	// There should be an army occupying this territory.
	assert(this->army != nullptr);
	assert(!this->army->isDead());

	// Update the positions of military unit sprites.
	updateMilitaryPosition();

	return isValid;  // WARNING CURRENTLY USELESS. IS THIS EVER NEEDED!!!
}

bool NavalTerritoryOccupancy::occupy(NavalFleet *fleet)
{
	bool isValid = false;

	if(this->fleet == nullptr)
	{
		this->fleet = fleet;
		fleet->setTerritory(&territory);
		isValid = true;
		// Observer methods.
		this->fleet->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
		territory.notifyObservers(newOccupant);
	}
	// Case armies have same owner.
	else if(&(fleet->getOwner()) == &(this->fleet->getOwner()))
	{
		const int initialStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();

		// Absorb strength of army into this->army.
		assert(fleet->getTotalStrength() > 0);
		this->fleet->increaseStrength(fleet->getStaminaStrength());
		fleet->clearStrength();  // Sets strength to 0.

		const int finalStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();
		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.

		isValid = true;
	}
	// Case armies have different owner.
	else
	{
		// Temporarily implemented as always attack. In future potentially not as there may be friendly sharing of troop territory.
//fleet->attack(*this->fleet, defenceMultiplier);
		// Attacking army occupys land if defending army killed.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->fleet == nullptr && !fleet->isDead())
		{
			this->fleet = fleet;
			fleet->setTerritory(&territory);
			isValid = true;
			// Observer methods.
			this->fleet->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
			territory.notifyObservers(newOccupant);
		}
	}

	// Remove pointer to army if army is dead.
	if(fleet != nullptr && fleet->isDead())
	{
		fleet = nullptr;
	}

	// Set the controller of this territory as the owner of the occupying naval army.
	if(this->fleet != nullptr)
	{
		assert(!this->fleet->isDead());
		controller = &this->fleet->getOwner();
	}

	if(&army->getOwner() != controller)
	{
		// Kill the army occupying this territory.
		army->clearStrength();  
		assert(army.isDead());
		army = nullptr;
	}

	// Update the positions of military unit sprites.
	updateMilitaryPosition();

	return isValid;  // WARNING CURRENTLY USELESS. IS THIS EVER NEEDED!!!
}

Player * NavalTerritoryOccupancy::getOccupant()
{
	return nullptr;
}

const LandArmy * NavalTerritoryOccupancy::getArmy() const
{
	return nullptr;
}

const NavalFleet * NavalTerritoryOccupancy::getFleet() const
{
	return nullptr;
}

void NavalTerritoryOccupancy::updateMilitaryPosition()
{
}
