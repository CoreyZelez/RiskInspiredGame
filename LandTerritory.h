#pragma once
#include "Territory.h"

class LandTerritory : public Territory
{
public:
	virtual bool transferArmy(LandArmy &army) override;
	virtual bool transferArmy(NavalArmy &army) override;

private:

};

