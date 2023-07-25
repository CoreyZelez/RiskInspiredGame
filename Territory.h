#pragma once
#include "IOccupy.h"
#include <memory>

class LandArmy;
class NavalFleet;

class Territory : public IOccupy
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is a success.
	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army) override = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is valid.
	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) override = 0;

	double getDefenceMultiplier() const;

private:
	double defenceMultiplier = 1.2;  // In future perhaps have complex virtual function to calculate this!
};

