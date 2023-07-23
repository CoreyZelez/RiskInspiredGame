#include "MilitaryForce.h"
#include <assert.h>

MilitaryForce::MilitaryForce(const Player &owner, Territory *location, unsigned int strength)
	: owner(owner), location(location), strength(strength)
{
}

bool MilitaryForce::move(Territory* location, unsigned int strength)
{
	assert(strength > 0);
	assert(strength <= getStrength());

	// Create army and move to target location.
	if(!createThenOccupy(location, strength))
	{
		return false;  // Return on transfer failure.
	}

	// Adjust strength of this military force.
	const int16_t adjustment = -static_cast<int16_t>(strength);
	assert(adjustment > 0);
	adjustStrength(adjustment);
	return true;
}

void MilitaryForce::adjustStrength(int amount)
{
	if(strength <= amount)
	{
		strength = 0;
		// Handle death here?
	}
	else
	{
		strength += amount;
	}
}

unsigned int MilitaryForce::getStrength() const
{
	return strength;
}

const Player& MilitaryForce::getOwner() const
{
	return owner;
}

Territory* MilitaryForce::getLocation() const
{
	return location;
}
