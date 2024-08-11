#pragma once
#include "ITerritoryOccupancy.h"
#include "Observer.h"

class SiegeManager;
class LandTerritory;
class NavalFleet;
class LandArmy;

class LandTerritoryOccupancy : public ITerritoryOccupancy, public Observer
{
public:
	explicit LandTerritoryOccupancy(LandTerritory &territory);

	void initSiegeManager(SiegeManager& siegeManager);

	virtual void update(Message message) override;

	// Determines and updates the controller of the territory.
	virtual void determineController();
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
	LandTerritory &territory;
	SiegeManager* siegeManager = nullptr;
	LandArmy *army;  // Army occupying territory. 

	// Sets positions of military unit sprites.
	void updateMilitaryPosition();

	// Sets new occupying army.
	void changeOccupyingArmy(LandArmy& army);

	// Sets controller. Returns true if controller changed.
	bool handleControllerChange(Player& player);
};

