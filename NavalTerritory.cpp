#include "NavalTerritory.h"
#include "Utility.h"
NavalTerritory::NavalTerritory()
	: Territory(createRandomNavalColor())
{
}

bool NavalTerritory::occupy(std::shared_ptr<LandArmy>& army)
{
	return false;
}

bool NavalTerritory::occupy(std::shared_ptr<NavalFleet>& fleet)
{
	return false;
}

void NavalTerritory::putArmy(std::shared_ptr<LandArmy>& army)
{
}

void NavalTerritory::putFleet(std::shared_ptr<NavalFleet>& fleet)
{
}
