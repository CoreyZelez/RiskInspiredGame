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
		setRuler(territory.getOccupant());
	}
}

void LandedEstate::yield(MilitaryManager &militaryManager)
{
	std::shared_ptr<LandArmy> landArmy = yieldLandArmy();
	std::shared_ptr<NavalFleet> navalFleet = yieldNavalFleet();
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

void LandedEstate::generateMilitary(MilitaryManager &militaryManager)
{
}

std::shared_ptr<LandArmy> LandedEstate::putArmy(int strength)
{
	// Should not be hostile army residing on territory.
	assert(territory.getOccupant() == nullptr || territory.getOccupant() == getRuler());

	std::shared_ptr<LandArmy> army = std::make_shared<LandArmy>(*getRuler(), &territory, strength);
	territory.occupy(army);

	// Army merged with pre-existing army on territory.
	if(army == nullptr)
	{
		return nullptr;
	}

	// There was no pre-existing army on territory.
	assert(army.get()->getStrength() > 0);
	return army;
}

std::shared_ptr<NavalFleet> LandedEstate::putFleet(int strength)
{
	std::shared_ptr<NavalFleet> fleet = std::make_shared<NavalFleet>(*getRuler(), &territory, strength);
	territory.occupy(fleet);
	return fleet;
}
