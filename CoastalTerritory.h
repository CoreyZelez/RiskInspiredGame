#pragma once
#include "LandTerritory.h"

class NavalFleet;

class CoastalTerritory : public LandTerritory
{
public:
	const Player &getOccupant() const;

private:
	std::shared_ptr<NavalFleet> fleet;  // Army occupying territory. 
};

