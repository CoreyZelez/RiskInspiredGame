#pragma once
#include "IOccupiable.h"
#include "Observer.h"

class LandTerritory;
class NavalFleet;
class LandArmy;

class LandTerritoryOccupancy : public IOccupiable, public Observer
{
public:
	explicit LandTerritoryOccupancy(LandTerritory &territory);

	virtual void update(Message message) override;

	// Determine occupyer.
	virtual void determineOccupation() override;
	// Handles land army occupation attempt. Returns true if successful.
	virtual bool occupy(LandArmy *army) override;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual bool occupy(NavalFleet *fleet) override;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) override;
	// Returns occupant of territory.
	virtual Player* getOccupant() override;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const override;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const override;

private:
	double defenceMultiplier = 1.4;  // In future perhaps have complex virtual function to calculate this!

	LandTerritory &territory;
	LandArmy *army;  // Army occupying territory. 
	NavalFleet *fleet;  // Fleet occupying territory coast. 

	// Sets positions of military unit sprites.
	void updateMilitaryPosition();
};

