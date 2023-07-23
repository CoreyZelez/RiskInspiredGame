#pragma once

class LandArmy;
class NavalFleet;

class Territory
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile.
	virtual bool occupy(LandArmy &army);
	// Army attempts to occupy this territory. Either peaceful or hostile.
	virtual bool occupy(NavalFleet &fleet);

private:

};

