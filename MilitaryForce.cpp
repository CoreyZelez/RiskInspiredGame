#include "MilitaryForce.h"
#include "Grid.h"
#include "Player.h"
#include "Territory.h"
#include <string>
#include <assert.h>
#include <iostream>

MilitaryForce::MilitaryForce(Player &owner, Territory *location, int strength, const sf::Texture &texture)
	: owner(owner), location(location), strength(strength), graphics(texture, this->strength)
{
	assert(location != nullptr);
	assert(strength > 0);
}

void MilitaryForce::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
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

	graphics.update();
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

void MilitaryForce::setSpritePosition(sf::Vector2f position)
{
	
	graphics.setPosition(position);
}
