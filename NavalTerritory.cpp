#include "NavalTerritory.h"
#include "Utility.h"
#include <assert.h>
#include <fstream>

NavalTerritory::NavalTerritory(TerritoryGraphics graphics)
	: Territory(graphics)
{
}

NavalTerritory::NavalTerritory()
	: Territory(createRandomNavalColor())
{
}

bool NavalTerritory::occupy(std::shared_ptr<LandArmy> &army)
{
	return false;
}

bool NavalTerritory::occupy(std::shared_ptr<NavalFleet> &fleet)
{
	return false;
}

std::string NavalTerritory::getSaveLabel() const
{
	return navalSaveLabel;
}

