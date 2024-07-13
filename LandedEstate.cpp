#include "LandedEstate.h"
#include "Territory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "MilitaryManager.h"
#include "LandTerritory.h"
#include "Player.h"
#include <fstream>
#include <iostream>

LandedEstate::LandedEstate(Title title, Territory &territory, sf::Color color)
	: Estate(title, territory.getGrid(), color), territory(territory)
{
	this->territory.addObserver(this);
	this->territory.assignLandedEstate(this);
}

void LandedEstate::yield()
{
	// Yields army if able to territory associated with estate.
	std::unique_ptr<LandArmy> landArmy = yieldLandArmy();
	// Yields fleet if able to territory associated with estate.
	std::unique_ptr<NavalFleet> navalFleet = yieldNavalFleet();
	if(landArmy != nullptr)
	{
		landArmy.get()->getOwner().getMilitaryManager().addLandArmy(std::move(landArmy));
	}
	if(navalFleet != nullptr)
	{
		navalFleet.get()->getOwner().getMilitaryManager().addNavalFleet(std::move(navalFleet));
	}
}

bool LandedEstate::hasPort() const
{
	const LandTerritory *landTerritory = dynamic_cast<LandTerritory*>(&territory);
	if(landTerritory != nullptr && landTerritory->hasPort())
	{
		return true;
	}
	return false;
}

bool LandedEstate::containsPosition(const sf::Vector2f &position) const
{
	return territory.getGrid().containsPosition(position);
}

int LandedEstate::getGridId() const
{
	// Landed estates have only a single subgrid in the associated composite grid.
	assert(getGrid().getGridIds().size() == 1);
	return getGrid().getGridIds()[0];
}

Territory& LandedEstate::getTerritory()
{
	return territory;
}

const Territory& LandedEstate::getTerritory() const
{
	return territory;
}

void LandedEstate::generateMilitary(MilitaryManager &militaryManager)
{
}

std::unique_ptr<LandArmy> LandedEstate::putArmy(int strength)
{
	// Territory must be controlled by player putting army.
	assert(territory.getController() == nullptr || territory.getController() == getRuler());
	assert(territory.getOccupancyHandler()->getArmy() == nullptr || &territory.getOccupancyHandler()->getArmy()->getOwner() == getRuler());

	// Yield army to upper liege.
	Player &upperLiege = getRuler()->getUpperLiege();
	std::unique_ptr<LandArmy> army = std::make_unique<LandArmy>(upperLiege, &territory, strength);
	territory.getOccupancyHandler()->occupy(army.get());

	// Army merged with pre-existing army on territory.
	if(army->isDead())
	{
		return nullptr;
	}

	// There was no pre-existing army on territory.
	assert(army.get()->getTotalStrength() > 0);
	return army;
}

std::unique_ptr<NavalFleet> LandedEstate::putFleet(int strength)
{
	// Intentionally does nothing.
	return nullptr;
}
