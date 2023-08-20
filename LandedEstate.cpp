#include "LandedEstate.h"
#include "Territory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "PlayerMilitaryManager.h"
#include "Player.h"

LandedEstate::LandedEstate(Title title, Territory &territory)
	: Estate(title, territory.getGrid()), territory(territory)
{
}

void LandedEstate::update(Message message)
{
	// Observed territory occupant changed.
	if(message == Message::newOccupant)
	{
		setRuler(territory.getOccupant());
	}
}

void LandedEstate::yield(PlayerMilitaryManager &militaryManager)
{
	militaryManager.addLandArmy(yieldLandArmy());
	militaryManager.addNavalFleet(yieldNavalFleet());
}

bool LandedEstate::containsPosition(const sf::Vector2f &position) const
{
	return territory.getGrid().containsPosition(position);
}

void LandedEstate::generateMilitary(PlayerMilitaryManager &militaryManager)
{
}

std::shared_ptr<LandArmy> LandedEstate::putArmy(int strength)
{
	std::shared_ptr<LandArmy> army = std::make_shared<LandArmy>(*getRuler(), &territory, strength);
	territory.occupy(army);
	return army;
}

std::shared_ptr<NavalFleet> LandedEstate::putFleet(int strength)
{
	std::shared_ptr<NavalFleet> fleet = std::make_shared<NavalFleet>(*getRuler(), &territory, strength);
	territory.occupy(fleet);
    return fleet;
}
