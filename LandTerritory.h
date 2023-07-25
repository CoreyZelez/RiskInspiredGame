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
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if occupy attempt is a success.
	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) override;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) override;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) override;

	bool isCoastal() const;

private:
	std::shared_ptr<LandArmy> army;  // Army occupying territory. 
	std::shared_ptr<NavalFleet> fleet;  // Fleet occupying coast assuming isCoastal true. 
	// In future above will probably be vector so multiple different occupants e.g. friendly nations or vassals.
};