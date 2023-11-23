#include "Maridom.h"
#include "NavalTerritory.h"

Maridom::Maridom(NavalTerritory &territory)
	: LandedEstate(Title::admiral, territory)
{
}
