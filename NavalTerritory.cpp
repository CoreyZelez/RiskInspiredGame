#include "NavalTerritory.h"
#include "Utility.h"
#include "NavalTerritoryOccupancy.h"
#include <assert.h>
#include <fstream>



NavalTerritory::NavalTerritory(int id, Grid grid)
	: Territory(id, grid, std::make_unique<NavalTerritoryOccupancy>(*this),  TerritoryType::naval)
{
}

NavalTerritory::NavalTerritory(int id)
	: Territory(id, createRandomNavalColor(), std::make_unique<NavalTerritoryOccupancy>(*this), TerritoryType::naval)
{
}

std::string NavalTerritory::getSaveLabel() const
{
	return navalSaveLabel;
}

