#pragma once
#include "LandTerritory.h"

class NavalFleet;

class CoastalTerritory : public LandTerritory
{
public:
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) override;

	const Player &getOccupant() const;

private:
	std::shared_ptr<NavalFleet> fleet;  // Army occupying territory. 
};

