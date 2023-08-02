#include "LandedEstate.h"
#include "Territory.h"

LandedEstate::LandedEstate(Title title, const Player *ruler, Territory &territory)
	: Estate(title, ruler), territory(territory)
{
}

void LandedEstate::update(Message message)
{
	if(message == Message::newOwner)
	{
		setRuler(territory.getOccupant());
	}
}

void LandedEstate::putArmy(int strength)
{

}

void LandedEstate::putFleet(int strength)
{
}
