#include "LandedEstate.h"
#include "Territory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "MilitaryManager.h"
#include "Player.h"
#include <fstream>
#include <iostream>

LandedEstate::LandedEstate(Title title, Territory &territory)
	: Estate(title, territory.getGrid()), territory(territory)
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
		setRuler(territory.getOccupancyHandler()->getOccupant());
	}
}

void LandedEstate::yield(MilitaryManager &militaryManager)
{
	// Yields army if able to territory associated with estate.
	std::unique_ptr<LandArmy> landArmy = yieldLandArmy();
	// Yields fleet if able to territory associated with estate.
	std::unique_ptr<NavalFleet> navalFleet = yieldNavalFleet();
	if(landArmy != nullptr)
	{
		militaryManager.addLandArmy(std::move(landArmy));
	}
	if(navalFleet != nullptr)
	{
		militaryManager.addNavalFleet(std::move(navalFleet));
	}
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

	std::unique_ptr<LandArmy> army = std::make_unique<LandArmy>(*getRuler(), &territory, strength);
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
	// Should not be hostile army residing on territory.
	assert(territory.getOccupancyHandler()->getOccupant() == nullptr
		|| territory.getOccupancyHandler()->getOccupant() == getRuler());

	std::unique_ptr<NavalFleet> fleet = std::make_unique<NavalFleet>(*getRuler(), &territory, strength);
	territory.getOccupancyHandler()->occupy(fleet.get());

	// Army merged with pre-existing army on territory.
	if(fleet->isDead())
	{
		return nullptr;
	}

	// There was no pre-existing army on territory.
	assert(fleet.get()->getTotalStrength() > 0);
	return fleet;
}
