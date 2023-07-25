#include "DirectEstate.h"

DirectEstate::DirectEstate(LandTerritory territory)
	: territory(territory)
{
}

bool DirectEstate::attemptOccupy(std::shared_ptr<NavalFleet>& fleet)
{
	return territory.attemptOccupy(fleet);
}

LandTerritory& DirectEstate::getTerritory()
{
	return territory;
}

bool DirectEstate::attemptOccupy(std::shared_ptr<LandArmy> &army)
{
	return territory.attemptOccupy(army);
}
