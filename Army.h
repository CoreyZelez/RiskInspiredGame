#pragma once
#include <cstdint>

class Player;
class Territory;

class Army
{
public:
	Army(const Player &owner, Territory *location, unsigned int strength);

	virtual bool move(Territory *location, unsigned int strength) = 0;

	void adjustStrength(int amount);
	unsigned int getStrength() const;

protected:
	const Player &getOwner() const;
	Territory *getLocation() const;

private:
	const Player &owner;
	Territory *location;
	uint16_t strength;
};

