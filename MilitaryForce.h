#pragma once
#include "MilitaryForceGraphics.h"
#include "Subject.h"
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <array>

class Player;
class Territory;

class MilitaryForce : public Subject
{
public:
	MilitaryForce(Player &owner, Territory *territory, unsigned int strength, const sf::Texture &texture);
	MilitaryForce(Player &owner, Territory *territory, std::array<unsigned int, 4> staminaStrength, const sf::Texture &texture);

	// Draws sprite at position of game world.
	virtual void draw(sf::RenderWindow &window) const;

	virtual void move(Territory &location, unsigned int strength) = 0;
	/*
	Moves to the territory along the path withing owners realm to the target territory.
	*/
	virtual void moveClosest(Territory &target, unsigned int strength, int maxDist) = 0;

	// Returns true if graphical component contains specified world position.
	bool containsPosition(sf::Vector2f position) const;
	void setSpritePosition(sf::Vector2f position);

	// Increment (decrement) strength by amount.
	void resetStamina();
	void reduceStrength(unsigned int amount);
	// Kills the unit by setting strength 0.
	void clearStrength();
	std::array<unsigned int, 4> getStaminaStrength() const;
	std::array<unsigned int, 4> expendStrength(unsigned int amount, const Territory &territory);
	void increaseStrength(unsigned int strengthAmount);
	void increaseStrength(std::array<unsigned int, 4> staminaStrength);
	unsigned int getStrength(int minStamina) const;
	unsigned int getTotalStrength() const;
	bool isDead() const;
	void checkDeath();

	Player &getOwner() const;
	Territory &getTerritory() const;
	void setTerritory(Territory *territory);

protected:
	virtual std::pair<int, int> calculateMinMaxStaminaCost(const Territory &territory) const = 0;

private:
	Player &owner;
	Territory *territory;  // Territory military resides at.
	std::array<unsigned int, 4> staminaStrength;
	MilitaryForceGraphics graphics;
};

Territory* nearestFriendlyAdjacentTerritoryDijkstra(Territory &sourceTerritory, Territory &targetTerritory, int maxDist);

