#pragma once
#include <cstdint>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(const Player &owner, Territory *location, int strength);

	virtual void move(Territory *location, int strength) = 0;

	// Getters and setters.
	void adjustStrength(int amount);
	int getStrength() const;
	bool isDead() const;
	const Player &getOwner();
	Territory *getLocation() const;
	void setLocation(Territory *location);

private:
	const Player &owner;
	Territory *location;
	int strength;
};

