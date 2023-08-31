#include "MilitaryForce.h"
#include "Grid.h"
#include "Player.h"
#include "Territory.h"
#include <string>
#include <assert.h>
#include <iostream>

MilitaryForce::MilitaryForce(Player &owner, Territory *territory, int strength, const sf::Texture &texture)
	: owner(owner), territory(territory), strength(strength), graphics(texture, this->strength)
{
	assert(territory != nullptr);
	assert(strength > 0);
}

void MilitaryForce::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

void MilitaryForce::adjustStrength(int amount)
{
	/// SHOULD NEVER ADJUST STRENGTH OF DEAD ARMY.
	assert(strength > 0);

	if(strength <= -amount)
	{
		strength = 0;
		notifyObservers(deadMilitary);
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

Territory& MilitaryForce::getTerritory() const
{
	return *territory;
}

void MilitaryForce::setTerritory(Territory* territory)
{
	this->territory = territory;
}

bool MilitaryForce::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

void MilitaryForce::setSpritePosition(sf::Vector2f position)
{
	
	graphics.setPosition(position);
}
