#pragma once
#include "MilitaryForce.h"

class NavalFleet : public MilitaryForce
{
public:
	NavalFleet(Player &owner, Territory *location, int strength);
	~NavalFleet() = default;

	virtual void move(Territory &location, int strength) override;
	virtual void moveClosest(Territory &target, int strength, int maxDist) override;

	// void attack(NavalFleet &defendingArmy, /// double defenceMultiplier);

private:

};