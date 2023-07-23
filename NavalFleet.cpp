#include "NavalFleet.h"
#include "Territory.h"
#include <assert.h>

NavalFleet::NavalFleet(const Player & owner, Territory * location, unsigned int strength)
	: MilitaryForce(owner, location, strength)
{
}

bool NavalFleet::createThenOccupy(Territory* location, unsigned int strength)
{
	NavalFleet army(getOwner(), getLocation(), strength);  // Army being transferred.
	return location->occupy(army); 
}
