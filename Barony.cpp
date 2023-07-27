#include "Barony.h"
#include "LandTerritory.h"

Barony::Barony(const Player *ruler, LandTerritory territory)
	: TerritoryEstate(baron, ruler, territory)
{
}
