#pragma once
#include "Estate.h"
#include "Observer.h"
#include "Territory.h"
#include "LandTerritory.h"

/*
 * TerritoryEstate refers to an estate with no lower titles. Necessarily associated with a territory.
 */
class TerritoryEstate : public Estate, public Observer
{
public:
	TerritoryEstate(Title title, const Player *ruler, LandTerritory &territory);
	~TerritoryEstate() = default;

	virtual void update(Message message) override;

	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army);  // Attempt occupy of underlying territory and estate.

	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet);  // Attempt occupy of underlying coast (port???).

	LandTerritory &getTerritory();  // Perhaps should return base class type?!?!

private:
	LandTerritory &territory;  
};

