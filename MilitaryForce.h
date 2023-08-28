#pragma once
#include "MilitaryForceGraphics.h"
#include <cstdint>
#include <SFML/Graphics.hpp>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(Player &owner, Territory *location, int strength, const sf::Texture &texture);

	// Draws sprite at position of game world.
	virtual void draw(sf::RenderWindow &window) const;

	virtual void move(Territory &location, int strength) = 0;

	// Getters and setters.
	void adjustStrength(int amount);
	int getStrength() const;
	bool isDead() const;
	Player &getOwner() const;
	Territory &getLocation() const;
	void setLocation(Territory *location);
	void setSpritePosition(sf::Vector2f position);

private:
	Player &owner;
	Territory *location;
	int strength;
	MilitaryForceGraphics graphics;
};
