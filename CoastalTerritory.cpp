#include "CoastalTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

void CoastalTerritory::putFleet(std::shared_ptr<NavalFleet>& fleet)
{
	// this->fleet must have same owner as army or be nullptr.
	assert(this->fleet.get() == nullptr || &this->fleet.get()->getOwner() == &fleet.get()->getOwner());
	if(this->fleet.get() == nullptr)
	{
		this->fleet = fleet;
	}
	else if(&this->fleet.get()->getOwner() == &fleet.get()->getOwner())
	{
		// Increment this->fleet strength by strength of fleet.
		this->fleet.get()->adjustStrength(fleet.get()->getStrength());
	}
}

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
