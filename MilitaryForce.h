#pragma once
#include <cstdint>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(const Player &owner, Territory *location, int strength);

	virtual void move(Territory *location, int strength) = 0;

	void adjustStrength(int amount);
	int getStrength() const;
	const Player &getOwner() const;
	bool isDead() const;
	Territory *getLocation() const;
	void setLocation(Territory *location);

private:
	const Player &owner;
	Territory *location;
	int strength;
};

