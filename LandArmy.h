#pragma once
#include "MilitaryForce.h"

class LandArmy : public MilitaryForce
{
public:
	LandArmy(const Player &owner, Territory *location, int strength);

	void attack(LandArmy &defendingArmy, float defenceMultiplier);

protected:
	virtual void move(Territory *location, int strength) override;

private:

};

