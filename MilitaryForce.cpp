#include "MilitaryForce.h"
#include "Grid.h"
#include <assert.h>

MilitaryForce::MilitaryForce(Player & owner, Territory * location, int strength, sf::Texture texture)
	: owner(owner), location(location), strength(strength)
{

	sprite.setTexture(texture);
	assert(location != nullptr);
	assert(strength > 0);
}

void MilitaryForce::adjustStrength(int amount)
{
	if(strength <= -amount)
	{
		strength = 0;
		// Handle death here?
	}
	else
	{
		strength += amount;
	}
}

int MilitaryForce::getStrength() const
{
	return strength;
}

Player& MilitaryForce::getOwner() const
{
	return owner;
}

bool MilitaryForce::isDead() const
{
	return strength == 0;
}

Territory& MilitaryForce::getLocation() const
{
	return *location;
}

void MilitaryForce::setLocation(Territory* location)
{
	this->location = location;
}
