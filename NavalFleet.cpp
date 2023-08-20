#include "NavalFleet.h"
#include "Territory.h"
#include <assert.h>
#include <memory>

NavalFleet::NavalFleet(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength)
{
	assert(location != nullptr);
}

void NavalFleet::move(Territory &location, int strength)
{
}

