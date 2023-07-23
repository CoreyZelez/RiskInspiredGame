#include "LandArmy.h"
#include "Territory.h"
#include <assert.h>

LandArmy::LandArmy(const Player &owner, Territory *location, unsigned int strength)
	: MilitaryForce(owner, location, strength)
{
}

bool LandArmy::createThenOccupy(Territory *location, unsigned int strength)
{
	LandArmy militaryForce(getOwner(), getLocation(), strength);  // Military force being transferred.
	return location->occupy(militaryForce);  // Send militar fForce to occupy location. Either hostile or non-hostile.
}




