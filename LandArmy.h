#pragma once
#include "Army.h"

class LandArmy : public Army
{
public:
	LandArmy(const Player &owner, Territory *location, unsigned int strength);

	virtual bool move(Territory *location, unsigned int strength) override;

private:

};

