#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(Player &owner, Territory *location, int strength);

	virtual void move(Territory &location, int strength) = 0;

	// Getters and setters.
	void adjustStrength(int amount);
	int getStrength() const;
	bool isDead() const;
	Player &getOwner() const;
	Territory &getLocation() const;
	void setLocation(Territory *location);

private:
	Player &owner;
	Territory *location;
	int strength;
};

