#pragma once

class LandArmy;
class NavalFleet;
class Player;

class IOccupiable
{
public:
	// Determine occupyer.
	virtual void determineOccupation() = 0;
	// Handles land army occupation attempt. Returns true if successful.
	virtual void occupy(LandArmy *army) = 0;
	// Handles navy fleet occupation attempt. Returns true if successful.
	virtual void occupy(NavalFleet *fleet) = 0;
	// Attempt to occupy until army dies or successfully occupies.
	virtual void forceOccupy(LandArmy *army) = 0;
	// Attempt to occupy until fleet dies or successfully occupies.
	virtual void forceOccupy(NavalFleet *fleet) = 0;
	// Returns current occupant of territory.
	virtual Player* getOccupant() = 0;
	// Returns land army occupying territory.
	virtual const LandArmy* getArmy() const = 0;
	// Returns naval fleet occupying territory.
	virtual const NavalFleet* getFleet() const = 0;
	// Removes army.
	virtual void removeArmy(const LandArmy* army) = 0;
	// Removes fleet.
	virtual void removeFleet(const NavalFleet* fleet) = 0;

protected:
	// Reevaluates the occupant. 
	virtual void reevaluateOccupancy() = 0;
};

