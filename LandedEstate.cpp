#include "LandedEstate.h"
#include "Territory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "MilitaryManager.h"
#include "LandTerritory.h"
#include "Player.h"
#include <fstream>
#include <iostream>

LandedEstate::LandedEstate(Title title, Territory &territory)
	: Estate(title, territory.getGrid()), territory(territory)
{
	this->territory.addObserver(this);
	this->territory.assignLandedEstate(this);
}

LandedEstate::LandedEstate(Title title, Territory & territory, sf::Color color)
	: Estate(title, territory.getGrid(), color), territory(territory)
{
	this->territory.addObserver(this);
	this->territory.assignLandedEstate(this);
}

void LandedEstate::saveToFile(std::ofstream & file) const
{
	Estate::saveToFile(file);
	file << "# territory id" << std::endl;
	file << territory.getID() << std::endl;
}

void LandedEstate::update(Message message)
{
	// Observed territory occupant changed.
	if(message == Message::newOccupant)
	{
		assert(territory.getOccupancyHandler()->getOccupant() != nullptr);
		setOwnership(territory.getOccupancyHandler()->getOccupant());
	}
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

Territory& LandedEstate::getTerritory()
{
	return territory;
}

void LandedEstate::generateMilitary(MilitaryManager &militaryManager)
{
}

std::unique_ptr<LandArmy> LandedEstate::putArmy(int strength)
{
	// Should not be hostile army residing on territory.
	assert(territory.getOccupancyHandler()->getOccupant() == nullptr 
		|| territory.getOccupancyHandler()->getOccupant() == getRuler());

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
