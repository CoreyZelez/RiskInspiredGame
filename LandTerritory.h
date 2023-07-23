#pragma once
#include "Territory.h"
#include "LandArmy.h"

class LandTerritory : public Territory
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile.
	virtual bool occupy(LandArmy &army) override;

private:
	LandArmy &army;  // army occupying territory.
};

