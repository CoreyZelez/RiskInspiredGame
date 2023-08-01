#pragma once
#include "Estate.h"

class Territory;

class LandedEstate : public Estate
{
public:
	LandedEstate(Title title, Player *ruler, Territory &territory);

private:
	Territory &territory;
};

