#pragma once
#include "ITerritoryOccupancy.h"
#include "Observer.h"

class LandTerritory;
class NavalFleet;
class LandArmy;

class LandTerritoryOccupancy : public ITerritoryOccupancy, public Observer
{
public:
	explicit LandTerritoryOccupancy(LandTerritory &territory);

	virtual void update(Message message) override;

	// Handles land army occupation attempt. Returns true if successful.
	virtual void occupy(LandArmy *army) override;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual void occupy(NavalFleet *fleet) override;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) override;
	// Attempt to occupy until fleet dies or successfully occupies.
	virtual void forceOccupy(NavalFleet *fleet) override;
	// Transfers control of the territory. Will then transfer back if previous controller military present.
	virtual void transferControl(Player& player) override;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const override;
	// Returns land army occupying territory.
	virtual LandArmy* getArmy() override;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const override;

private:
	LandTerritory &territory;
	LandArmy *army;  // Army occupying territory. 

	// Sets positions of military unit sprites.
	void updateMilitaryPosition();

	// Sets new occupying army.
	void changeOccupyingArmy(LandArmy *army);
};

