#pragma once
#include "Estate.h"
#include "Territory.h"
#include "LandTerritory.h"

/*
 * DirectEstate refers to an estate with no lower titles. Necessarily associated with a territory.
 */
class DirectEstate 
{
public:
	DirectEstate(LandTerritory territory);

	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army);  // Attempt occupy of underlying territory and estate.

	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet);  // Attempt occupy of underlying coast (port???).

	LandTerritory &getTerritory();  // Perhaps should return base class type?!?!

private:
	LandTerritory territory;  
};

