#pragma once

class LandArmy;
class NavalArmy;

class Territory
{
public:
	virtual bool transferArmy(LandArmy &army) = 0;
	virtual bool transferArmy(NavalArmy &army) = 0;

private:

};

