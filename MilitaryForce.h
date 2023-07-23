#pragma once
#include <cstdint>

class Player;
class Territory;

class MilitaryForce
{
public:
	MilitaryForce(const Player &owner, Territory *location, unsigned int strength);

	bool move(Territory *location, unsigned int strength);

	void adjustStrength(int amount);
	unsigned int getStrength() const;

protected:
	// Create army with given strength then attempt to occupy location.
	virtual bool createThenOccupy(Territory *location, unsigned int strength) = 0;  

	const Player &getOwner() const;
	Territory *getLocation() const;

private:
	const Player &owner;
	Territory *location;
	uint16_t strength;
};

