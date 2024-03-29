#pragma once
#include "IOccupiable.h"
#include "Observer.h"
#include "NavalTerritory.h"

class NavalTerritoryOccupancy : public IOccupiable, public Observer
{
public:
	explicit NavalTerritoryOccupancy(NavalTerritory &territory);

	virtual void update(Message message) override;

	// Determine occupyer.
	virtual void determineOccupation() override;
	// Handles land army occupation attempt. Returns true if successful.
	virtual void occupy(LandArmy *army) override;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual void occupy(NavalFleet *fleet) override;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) override;
	// Attempt to occupy until fleet dies or successfully occupies.
	virtual void forceOccupy(NavalFleet *fleet) override;
	// Returns occupant of territory.
	virtual Player* getOccupant() override;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const override;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const override;
	// Removes army.
	virtual void removeArmy(const LandArmy* army) override;
	// Removes fleet.
	virtual void removeFleet(const NavalFleet* fleet) override;

protected:
	// Reevaluates the occupancy. 
	virtual void reevaluateOccupancy() override;

private:
	double defenceMultiplier = 1.4;  // In future perhaps have complex virtual function to calculate this!

	NavalTerritory &territory;
	LandArmy *army;  // Army occupying territory. 
	NavalFleet *fleet;  // Fleet occupying territory coast. 
	Player *mostRecentOccupant;  // Most recent player to have a naval unit occupying this territory.

	// Sets positions of military unit sprites.
	void updateMilitaryPosition();
};

