#pragma once
#include <memory>

class LandArmy;
class NavalFleet;

class Territory 
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is a success.
	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is valid.
	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) = 0;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) = 0;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) = 0;

	double getDefenceMultiplier() const;

private:
	double defenceMultiplier = 1.2;  // In future perhaps have complex virtual function to calculate this!
};

