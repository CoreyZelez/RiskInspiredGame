#include "CoastalTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

const Player& CoastalTerritory::getOccupant() const
{
	assert(getArmy().get() != nullptr || fleet.get() != nullptr);
	if(getArmy().get() != nullptr)
	{
		return getArmy().get()->getOwner();
	}
	else if(fleet.get() != nullptr)
	{
		return fleet.get()->getOwner();
	}
}



