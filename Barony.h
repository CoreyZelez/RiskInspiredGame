#pragma once
#include "TerritoryEstate.h"
#include "LandTerritory.h"
#include <memory>

class Barony : public TerritoryEstate
{
public:
	Barony(const Player *ruler, LandTerritory territory);
	~Barony() = default;

private:

};

