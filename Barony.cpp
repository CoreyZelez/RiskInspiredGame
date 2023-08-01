#include "Barony.h"
#include "LandTerritory.h"
#include "CoastalTerritory.h"

Barony::Barony(Player *player, LandTerritory &territory)
	: LandedEstate(Title::baron, player, territory)
{
}

Barony::Barony(Player *ruler, CoastalTerritory &territory)
	: LandedEstate(Title::baron, ruler, territory)
{
}
