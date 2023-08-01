#pragma once
#include "LandedEstate.h"
#include "LandTerritory.h"
#include <memory>

class CoastalTerritory;
class LandTerritory;

class Barony : public LandedEstate
{
public:
	Barony(Player *ruler, LandTerritory &territory);
	Barony(Player *ruler, CoastalTerritory &territory);

	~Barony() = default;

private:


};

