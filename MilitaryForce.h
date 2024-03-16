#pragma once
#include "MilitaryForceGraphics.h"
#include "Subject.h"
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <array>

class Player;
class Territory;
enum class TerritoryType;

const int MAX_STAMINA = 2;
using StaminaArray = std::array<unsigned int, MAX_STAMINA + 1>;

class MilitaryForce : public Subject
{
public:
	MilitaryForce(Player &owner, Territory *territory, unsigned int strength, const std::string &shape);
	MilitaryForce(Player &owner, Territory *territory, StaminaArray staminaStrength, const std::string &shape);

	virtual void removeFromTerritory() = 0;

	// Draws sprite at position of game world.
	virtual void draw(sf::RenderWindow &window) const;

	virtual void move(Territory &location, unsigned int strength) = 0;
	
	//Moves to the territory along the path withing owners realm to the target territory.
	virtual void moveClosest(Territory &target, unsigned int strength, int maxDist) = 0;

	// Returns true if graphical component contains specified world position.
	bool containsPosition(sf::Vector2f position) const;
	void setSpritePosition(sf::Vector2f position);

	// Increment (decrement) strength by amount.
	void resetStamina();
	void reduceStrength(unsigned int amount);
	// Kills the unit by setting strength 0.
	void clearStrength();
	std::array<unsigned int, 3> getStaminaStrength() const;
	std::array<unsigned int, 3> expendStrength(unsigned int amount, const Territory &territory);
	void increaseStrength(unsigned int strengthAmount);
	void increaseStrength(std::array<unsigned int, 3> staminaStrength);
	unsigned int getStrength(int minStamina) const;
	unsigned int getTotalStrength() const;
	bool isDead() const;
	void checkDeath();

	Player &getOwner() const;
	Territory &getTerritory();
	const Territory &getTerritory() const;
	void setTerritory(Territory *territory);

protected:
	// Updates diplomacy of players involved in troop movement.
	void updatePlayerDiplomacy(Player *locationEstateOwner);

	virtual std::pair<int, int> calculateMinMaxStaminaCost(const Territory &territory) const = 0;

	int getMaximumStamina() const;

private:
	Player &player;
	Territory *territory;  // Territory military resides at.
	StaminaArray staminaStrength;
	MilitaryForceGraphics graphics;
};

Territory* nearestFriendlyAdjacentTerritoryDijkstra(Territory &sourceTerritory, Territory &targetTerritory, int maxDist);

Territory* nearestFriendlyAdjacentTerritoryDijkstra(Territory &sourceTerritory, Territory &targetTerritory, int maxDist, TerritoryType territoryType);
