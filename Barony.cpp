#include "Barony.h"
#include "LandTerritory.h"
#include "CoastalTerritory.h"

Barony::Barony(Player* ruler, LandTerritory &territory, double landArmyYield)
	: LandedEstate(Title::baron, ruler, territory), 
	landArmyYield(landArmyYield)
{
}

Barony::Barony(Player* ruler, CoastalTerritory& territory, double landArmyYield, double navalFleetYield)
	: LandedEstate(Title::baron, ruler, territory), canYieldNavy(true), 
	landArmyYield(landArmyYield), navalFleetYield(navalFleetYield)
{
}

