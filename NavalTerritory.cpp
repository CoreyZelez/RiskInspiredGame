#include "NavalTerritory.h"
#include "Utility.h"
#include <assert.h>
#include <fstream>



NavalTerritory::NavalTerritory(int id, Grid grid)
	: Territory(id, grid)
{
}

NavalTerritory::NavalTerritory(int id)
	: Territory(id, createRandomNavalColor())
{
}

std::string NavalTerritory::getSaveLabel() const
{
	return navalSaveLabel;
}

