#include "Army.h"

Army::Army(const Player &owner, Territory *location, unsigned int strength)
	: owner(owner), location(location), strength(strength)
{
}

void Army::adjustStrength(int amount)
{
	strength += amount;
}

unsigned int Army::getStrength() const
{
	return strength;
}

const Player& Army::getOwner() const
{
	return owner;
}

Territory* Army::getLocation() const
{
	return location;
}
