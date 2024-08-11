#pragma once
#include "ITerritoryOccupancy.h"
#include "Observer.h"
#include "NavalTerritory.h"

class Maridom;

class NavalTerritoryOccupancy : public ITerritoryOccupancy, public Observer
{
public:
	explicit NavalTerritoryOccupancy(NavalTerritory& territory);

	void initMaridom(Maridom& maridom);

	virtual void update(Message message) override;

	// Determines and updates the controller of the territory.
	virtual void determineController() override;
	// Handles land army occupation attempt. Returns true if successful.
	virtual void occupy(LandArmy *army) override;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual void occupy(NavalFleet *fleet) override;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) override;
	// Attempt to occupy until fleet dies or successfully occupies.
	virtual void forceOccupy(NavalFleet *fleet) override;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const override;
	// Returns land army occupying territory.
	virtual LandArmy* getArmy() override;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const override;

private:
	double defenceMultiplier = 1.4;  // In future perhaps have complex virtual function to calculate this!

	NavalTerritory &territory;
	Maridom* maridom = nullptr;
	LandArmy *army;  // Army occupying territory. 
	NavalFleet *fleet;  // Fleet occupying territory coast. 

	// Sets new occupying fleet.
	void changeOccupyingFleet(NavalFleet* fleet);

	bool handleControllerChange(Player& player);

	// Sets positions of military unit sprites.
	void updateMilitaryPosition();
};

