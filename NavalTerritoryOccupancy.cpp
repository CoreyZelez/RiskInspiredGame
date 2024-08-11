#include "NavalTerritoryOccupancy.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "Player.h"
#include "Maridom.h"
#include <assert.h>
#include <iostream>

NavalTerritoryOccupancy::NavalTerritoryOccupancy(NavalTerritory& territory)
	: territory(territory)
{
}

void NavalTerritoryOccupancy::initMaridom(Maridom& maridom)
{
	this->maridom = &maridom;
}

void NavalTerritoryOccupancy::update(Message message)
{
	switch(message)
	{
	case vacatedMilitary:
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

void NavalTerritoryOccupancy::determineController()
{
	// Implementation not required.
	assert(false);
}

void NavalTerritoryOccupancy::occupy(LandArmy *army)
{
	// Can successfully occupy with army if army owner is the most recent controller of territory by navy.
	if(controller == &army->getOwner())
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
		changeOccupyingFleet(fleet);
	}
	else if(&(fleet->getOwner()) == &(this->fleet->getOwner()))
	{
		// Case fleets have same owner.

		assert(fleet != this->fleet);

		const int initialStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();

		this->fleet->increaseStrength(fleet->getStaminaStrength());
		fleet->clearStrength(); 

		const int finalStrengthSum = fleet->getTotalStrength() + this->fleet->getTotalStrength();
		assert(initialStrengthSum == finalStrengthSum);  // Strength sum should remain unchanged.
	}
	else
	{
		// Case fleets have different owner.

		// Temporarily implemented as always attack. 
        fleet->attack(*this->fleet, defenceMultiplier);

		// Attacking fleet occupys territory if defending fleet killed.
		// this->fleet nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->fleet == nullptr && !fleet->isDead())
		{
			changeOccupyingFleet(fleet);
		}
	}

	// Set the controller of this territory as the owner of the occupying naval army.
	// Notify observers if owner changed.
	if(this->fleet != nullptr && controller != &this->fleet->getOwner())
	{
		controller = &this->fleet->getOwner();
	}

	// Remove pointer to army if army is dead.
	// Note that the owner should still be the owner of the now dead fleet.
	if(this->fleet != nullptr && this->fleet->isDead())
	{
		this->fleet = nullptr;
	}

	// Kill enemy land armies occupying this territory.
	if((army != nullptr) && (&army->getOwner() != controller))
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

void NavalTerritoryOccupancy::forceOccupy(NavalFleet *fleet)
{
	// Attempt occupancy.
	territory.getOccupancyHandler()->occupy(fleet);

	// Repeatedly reattempy occupancy of naval territory until success or death of fleet.
	// This is necessary since first attempt to occupy may fail whilst fleet still alive thus has no where to return to.
	while(!fleet->isDead() && controller != &fleet->getOwner())
	{
		occupy(fleet);
	}
}

const LandArmy* NavalTerritoryOccupancy::getArmy() const
{
	return army;
}

LandArmy* NavalTerritoryOccupancy::getArmy()
{
	return army;
}

const NavalFleet* NavalTerritoryOccupancy::getFleet() const
{
	return fleet;
}

void NavalTerritoryOccupancy::changeOccupyingFleet(NavalFleet* fleet)
{
	assert(this->fleet == nullptr);

	this->fleet = fleet;
	fleet->setTerritory(&territory);

	handleControllerChange(fleet->getOwner());

	// Observer methods.
	this->fleet->addObserver(this);  // Observe army so it can be set to nullptr when it dies.

	maridom->setOwnership(&fleet->getOwner());
}

bool NavalTerritoryOccupancy::handleControllerChange(Player& player)
{
	if(&player == controller)
	{
		assert(player.getRealm().getTerritories().controlsTerritory(territory));
		return false;
	}
	else if(!sameUpperRealm(controller, &player))
	{
		if(controller != nullptr)
		{
			controller->getRealm().removeControl(territory);
		}
		player.getRealm().addControl(territory);
		controller = &player;

		return true;
	}
	else
	{
		if(!player.isVassal(*controller, false))
		{
			// Controller is apart of same realm as player but is not player's liege. Thus controller loses control
			// of the territory.
			controller->getRealm().removeControl(territory);
		}

		if(!player.getRealm().getTerritories().controlsTerritory(territory))
		{
			player.getRealm().addControl(territory);
		}

		controller = &player;

		return true;
	}

	return false;
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
