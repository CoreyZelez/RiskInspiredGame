#pragma once
#include "Territory.h"
#include <memory>

class LandArmy;
class NavalFleet;

class LandTerritory : public Territory
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is a success.
	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army) override;
	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) override;

private:
	std::shared_ptr<LandArmy> army;  // Army occupying territory. 
	// In future above will probably be vector so multiple different occupants e.g. friendly nations or vassals.
};

