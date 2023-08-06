#include "LandedEstate.h"
#include "Territory.h"
#include "NavalFleet.h"
#include "LandArmy.h"

LandedEstate::LandedEstate(Title title, const Player *ruler, Territory &territory)
	: Estate(title, ruler), territory(territory)
{
}

void LandedEstate::update(Message message)
{
	if(message == Message::newOccupant)
	{
		setRuler(territory.getOccupant());
	}
}

std::shared_ptr<LandArmy> LandedEstate::putArmy(int strength)
{
	//std::shared_ptr<LandArmy> army = std::make_shared<LandArmy>(*getRuler(), territory, strength);
	//territory.occupy(army);
	//return army;
	return nullptr;
}

std::shared_ptr<NavalFleet> LandedEstate::putFleet(int strength)
{
	//std::shared_ptr<NavalFleet> fleet = std::make_shared<NavalFleet>(*getRuler(), territory, strength);
	//territory.occupy(fleet);
	//return fleet;
	return nullptr;
}
