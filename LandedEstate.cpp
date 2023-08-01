#include "LandedEstate.h"

LandedEstate::LandedEstate(Title title, Player *ruler, Territory &territory)
	: Estate(title, ruler), territory(territory)
{
}
