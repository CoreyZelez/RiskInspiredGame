#include "NavalTerritoryOccupancy.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "Player.h"
#include <assert.h>
#include <iostream>

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

void NavalTerritoryOccupancy::determineOccupation()
{

	if(fleet != nullptr)
	{
		const Player& fleetOwner = fleet->getOwner();
		if(!sameUpperRealm(mostRecentOccupant, &fleetOwner))
		{
			territory.notifyObservers(newOccupant);
		}
	}
}

void NavalTerritoryOccupancy::occupy(LandArmy *army)
{
	// Can successfully occupy with army if army owner is the most recent controller of territory by navy.
	if(this->mostRecentOccupant == &army->getOwner())
	{
		if(this->army == nullptr)
		{
			this->army = army;
			army->setTerritory(&territory);

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
		}
	}

	// There should be an army occupying this territory.
	assert(this->army != nullptr);
	assert(!this->army->isDead());

	// Update the positions of military unit sprites.
	updateMilitaryPosition();
}

void NavalTerritoryOccupancy::occupy(NavalFleet *fleet)
{
	assert(fleet != nullptr);

	if(this->fleet == nullptr)
	{
		this->fleet = fleet;
		this->fleet->setTerritory(&territory);

		// Observer methods.
		this->fleet->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
	}
	// Case fleets have same owner.
	else if(&(fleet->getOwner()) == &(this->fleet->getOwner()))
	{
		assert(fleet != this->fleet);
		const int initialStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();
		// Absorb strength of army into this->fleet.
		assert(fleet->getTotalStrength() > 0);
		this->fleet->increaseStrength(fleet->getStaminaStrength());
		fleet->clearStrength();  // Sets strength to 0.
		const int finalStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();
		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.
	}
	// Case fleets have different owner.
	else
	{
		// Temporarily implemented as always attack. In future potentially not as there may be friendly sharing of troop territory.
        fleet->attack(*this->fleet, defenceMultiplier);
		// Attacking fleet occupys territory if defending fleet killed.
		// this->fleet nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->fleet == nullptr && !fleet->isDead())
		{
			this->fleet = fleet;
			this->fleet->setTerritory(&territory);

			// Observer methods.
			this->fleet->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
		}
	}

	// Set the controller of this territory as the owner of the occupying naval army.
	// Notify observers if owner changed.
	if(this->fleet != nullptr && mostRecentOccupant != &this->fleet->getOwner())
	{
		mostRecentOccupant = &this->fleet->getOwner();
		territory.notifyObservers(newOccupant);
	}

	// Remove pointer to army if army is dead.
	// Note that the owner should still be the owner of the now dead fleet.
	if(this->fleet != nullptr && this->fleet->isDead())
	{
		this->fleet = nullptr;
	}

	// Kill enemy land armies occupying this territory.
	if((army != nullptr) && (&army->getOwner() != mostRecentOccupant))
	{
		assert(!army->isDead());
		// Kill the army occupying this territory.
		army->clearStrength();  
		army = nullptr;
	}

	// Update the positions of military unit sprites.
	updateMilitaryPosition();
}

void NavalTerritoryOccupancy::forceOccupy(LandArmy *army)
{
	// Intentionally empty.
}

void NavalTerritoryOccupancy::forceOccupy(NavalFleet * fleet)
{
	// Attempt occupancy.
	territory.getOccupancyHandler()->occupy(fleet);
	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!fleet->isDead() && !sameUpperRealm(territory.getOccupancyHandler()->getOccupant(), &fleet->getOwner()))
	{
		occupy(fleet);
	}
}

Player * NavalTerritoryOccupancy::getOccupant()
{
	return mostRecentOccupant;

}

const LandArmy* NavalTerritoryOccupancy::getArmy() const
{
	return army;
}

const NavalFleet* NavalTerritoryOccupancy::getFleet() const
{
	return fleet;
}

void NavalTerritoryOccupancy::removeArmy(const LandArmy *army)
{
	assert(this->army == army);
	this->army = nullptr;
	reevaluateOccupancy();
}

void NavalTerritoryOccupancy::removeFleet(const NavalFleet *fleet)
{
	assert(this->fleet == fleet);
	this->fleet = nullptr;
	reevaluateOccupancy();
}

void NavalTerritoryOccupancy::reevaluateOccupancy()
{
	if(army == nullptr && fleet == nullptr)
	{
		mostRecentOccupant = nullptr;
	}
}

void NavalTerritoryOccupancy::updateMilitaryPosition()
{
	const int offset = 95;
	if(army != nullptr)
	{
		sf::Vector2f pos = territory.getGrid().getCenter();
		pos.y += offset;
		army->setSpritePosition(pos);
	}
	if(fleet != nullptr)
	{
		sf::Vector2f pos = territory.getGrid().getCenter();
		pos.y -= offset;
		fleet->setSpritePosition(pos);
	}
}
