#pragma once
#include "MilitaryForceGraphics.h"
#include <cstdint>
#include <SFML/Graphics.hpp>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(Player &owner, Territory *territory, int strength, const sf::Texture &texture);

	// Draws sprite at position of game world.
	virtual void draw(sf::RenderWindow &window) const;

	virtual void move(Territory &location, int strength) = 0;

	// Returns true if graphical component contains specified world position.
	bool containsPosition(sf::Vector2f position) const;
	void setSpritePosition(sf::Vector2f position);
	// Increment (decrement) strength by amount.
	void adjustStrength(int amount);
	int getStrength() const;
	bool isDead() const;
	Player &getOwner() const;
	Territory &getTerritory() const;
	void setTerritory(Territory *territory);

private:
	Player &owner;
	Territory *territory;  // Territory military resides at.
	int strength;
	MilitaryForceGraphics graphics;
};
