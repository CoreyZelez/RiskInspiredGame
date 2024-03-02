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

void LandTerritoryOccupancy::determineOccupation()
{
	const Player *estateOwner = territory.getEstateOwner();
	if(army != nullptr)
	{
		const Player& armyOwner = army->getOwner();
		if(!sameUpperRealm(estateOwner, &armyOwner))
		{
			territory.notifyObservers(newOccupant);
		}
	}
}

void LandTerritoryOccupancy::occupy(LandArmy *army)
{
	bool isValid = false;

	// Case no pre existing occupying army on terriory
	if(this->army == nullptr)
	{
		// Determine whether occupation is friendly or hostile.
		const Player *estateOwner = territory.getEstateOwner();
		const Player *armyOwner = &army->getOwner();
		bool friendly = sameUpperRealm(estateOwner, armyOwner);
		if(friendly) 
		{
			updateOccupyingArmy(army);
		}
		else
		{
			const int occupancyCost = army->getOwner().getGameplaySettings().landHostileOccupancyCost;

			// Army occupies territory and is applied occupancy strength reduction cost.
			if(army->getTotalStrength() >= occupancyCost)
			{
				updateOccupyingArmy(army);
				applyArmyOccupationCost();
			}
			else
			{
				// Army not strong enough to occupy. Strength returned to source territory, see move function.			
				// Do nothing.
			}
		}
		
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
		//const double defenceMultiplier = territory.getFeatures().terrain.defenceMultiplier;
		army->attack(*this->army, territory.getTerrain());

		// Attacking army occupys land if defending army killed and strength at occupancy cost threshold.
		// this->army nullptr implies dead since it's pointer was nulled by territory in response to its death.
		if(this->army == nullptr && !army->isDead())
		{
			const int occupancyCost = army->getOwner().getGameplaySettings().landHostileOccupancyCost;

			// Army occupies territory and is applied occupancy strength reduction cost.
			if(army->getTotalStrength() >= occupancyCost)
			{
				updateOccupyingArmy(army);
				applyArmyOccupationCost();
			}
			else
			{
				// Army not strong enough to occupy. Strength returned to source territory, see move function.			
				// Do nothing.
			}
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
	while(!army->isDead() && !sameUpperRealm(territory.getOccupancyHandler()->getOccupant(), &army->getOwner()))
	{
		occupy(army);
	}
}

void LandTerritoryOccupancy::forceOccupy(NavalFleet * fleet)
{
	// Intentionally empty.
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

void LandTerritoryOccupancy::removeArmy(const LandArmy *army)
{
	assert(this->army == army);
	this->army = nullptr;
	reevaluateOccupancy();
}

void LandTerritoryOccupancy::removeFleet(const NavalFleet *fleet)
{
	assert(this->fleet == fleet);
	this->fleet = nullptr;
	reevaluateOccupancy();
}

void LandTerritoryOccupancy::reevaluateOccupancy()
{
	// Intentionally empty.
	// Land territory occupant is solely determined by stationed army if any.
	// In future may have to implement if occupancy remains even when army no longer on territory.
}

void LandTerritoryOccupancy::updateMilitaryPosition()
{
	if(army != nullptr)
	{
		army->setSpritePosition(territory.getGrid().getCenter());
	}
}

void LandTerritoryOccupancy::updateOccupyingArmy(LandArmy *army)
{
	this->army = army;
	army->setTerritory(&territory);

	// Observer methods.
	this->army->addObserver(this);  // Observe army so it can be set to nullptr when it dies.
	territory.notifyObservers(newOccupant);
}

void LandTerritoryOccupancy::applyArmyOccupationCost()
{
	const int occupancyCost = army->getOwner().getGameplaySettings().landHostileOccupancyCost;

	// Reduce strength of army due to occupancy cost. Army possibly dies.
	this->army->reduceStrength(occupancyCost);
}
