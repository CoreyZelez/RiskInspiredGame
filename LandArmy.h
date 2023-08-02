#pragma once
#include "MilitaryForce.h"

class LandArmy : public MilitaryForce
{
public:
	LandArmy(const Player &owner, Territory *location, int strength);
	~LandArmy() = default;

	void attack(LandArmy &defendingArmy, double defenceMultiplier);
	virtual void move(Territory *location, int strength) override;

private:

};

