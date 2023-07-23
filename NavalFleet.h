#pragma once
#include "MilitaryForce.h"

class NavalFleet : public MilitaryForce
{
public:
	NavalFleet(const Player &owner, Territory *location, unsigned int strength);

protected:
	virtual bool createThenOccupy(Territory *location, unsigned int strength) override;

private:

};