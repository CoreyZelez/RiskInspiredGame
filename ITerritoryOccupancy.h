#pragma once

class LandArmy;
class NavalFleet;
class Player;

class ITerritoryOccupancy
{
public:
	// Handles land army occupation attempt. Returns true if successful.
	virtual void occupy(LandArmy *army) = 0;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual void occupy(NavalFleet *fleet) = 0;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) = 0;
	// Attempt to occupy until fleet dies or successfully occupies.
	virtual void forceOccupy(NavalFleet *fleet) = 0;
	// Returns controller of territory. Controller will either be the player who last occupied the territory
	// through military, or the player who owns the associated land estate of the territory. The controller 
	// must not have a liege.
	virtual const Player* getController() const; 
	// Returns controller of territory. Controller will either be the player who last occupied the territory
	// through military, or the player who owns the associated land estate of the territory. The controller 
	// must not have a liege.
	virtual Player* getController();
	// Transfers control of the territory. Will then transfer back if previous controller military present.
	virtual void transferControl(Player& player) = 0;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const = 0;
	// Returns land army occupying territory.
	virtual LandArmy* getArmy() = 0;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const = 0;

protected:
	Player* controller = nullptr;

};

