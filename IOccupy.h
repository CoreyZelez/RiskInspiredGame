#pragma once
#include <memory>

class LandArmy;
class NavalFleet;

/*
 * Interface for providing occupy capabilites for military classes.
 */
class IOccupy
{
public:
	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army) = 0;
	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) = 0;
};
