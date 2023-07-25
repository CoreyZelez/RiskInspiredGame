#pragma once
#include "Estate.h"
#include "Territory.h"

/*
 * DirectEstate refers to an estate with no lower titles. Necessarily associated with a territory.
 */
class DirectEstate : public Estate
{
public:
	Territory &getTerritory();

private:
	Territory territory;
};

