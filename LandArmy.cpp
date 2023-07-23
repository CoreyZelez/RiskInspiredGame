#include "LandArmy.h"
#include "Territory.h"
#include <assert.h>

LandArmy::LandArmy(const Player &owner, Territory *location, unsigned int strength)
	: Army(owner, location, strength)
{
}

bool LandArmy::move(Territory *location, unsigned int strength)
{
	assert(strength > 0);
	assert(strength <= getStrength());

	// Transfer part of this army to target location.
	LandArmy army(getOwner(), getLocation(), strength);  // Army being transferred.
	bool transferSuccess = location->transferArmy(army);
	if(!transferSuccess)
	{
		return false;
	}

	// Adjust strength of this army.
	const int16_t adjustment = -static_cast<int16_t>(strength);
	assert(adjustment > 0);
	adjustStrength(adjustment);
	return true;
}
