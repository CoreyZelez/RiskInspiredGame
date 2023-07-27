#include "TerritoryEstate.h"
#include "LandTerritory.h"

TerritoryEstate::TerritoryEstate(Title title, const Player *ruler, LandTerritory &territory)
	: Estate(title, ruler), territory(territory)
{
	territory.addObserver(this);
}

bool TerritoryEstate::attemptOccupy(std::shared_ptr<NavalFleet>& fleet)
{
	return territory.occupy(fleet);
}

LandTerritory& TerritoryEstate::getTerritory()
{
	return territory;
}

void TerritoryEstate::update(Message message)
{
	switch(message)
	{
	case newOwner:
		changeRuler(&territory.getOccupant());
	}
}

bool TerritoryEstate::attemptOccupy(std::shared_ptr<LandArmy> &army)
{
	return territory.occupy(army);
}
