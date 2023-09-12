#pragma once
#include "MilitaryForce.h"

class LandArmy : public MilitaryForce
{
public:
	LandArmy(Player &owner, Territory *location, int strength);
	~LandArmy() = default;

	virtual void move(Territory &territory, int strength) override;
	virtual void moveClosest(Territory &target, int strength) override;

	void attack(LandArmy &defendingArmy, double defenceMultiplier);

private:

};

