#pragma once
#include "MilitaryForce.h"

class NavalFleet : public MilitaryForce
{
public:
	NavalFleet(const Player &owner, Territory *location, int strength);

protected:

private:

};