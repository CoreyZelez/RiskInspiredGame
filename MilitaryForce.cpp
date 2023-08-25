#include "MilitaryForce.h"
#include "Grid.h"
#include "Player.h"
#include "Territory.h"
#include <assert.h>

MilitaryForce::MilitaryForce(Player &owner, Territory *location, int strength)
	: owner(owner), location(location), strength(strength)
{

	assert(location != nullptr);
	assert(strength > 0);
}

MilitaryForce::MilitaryForce(Player &owner, Territory *location, int strength, const sf::Texture &texture)
	: owner(owner), location(location), strength(strength)
{
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));
	sprite.setTexture(texture);
	assert(location != nullptr);
	assert(strength > 0);
}

void MilitaryForce::draw(sf::RenderWindow &window) const
{
	window.draw(sprite);
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
	sprite.setPosition(location->getCenter());  // Temporary. Will have to draw off center in future due to multiple armies at single territory.
}
