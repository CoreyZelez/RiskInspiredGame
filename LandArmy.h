#pragma once
#include "MilitaryForce.h"

class LandArmy : public MilitaryForce
{
public:
	LandArmy(const Player &owner, Territory *location, unsigned int strength);

protected:
	virtual bool createThenOccupy(Territory *location, unsigned int strength) override;

private:

};

