#include "Territory.h"
#include <assert.h>

void Territory::initEstate(const TerritoryEstate *estate)
{
	assert(this->estate == nullptr);
	this->estate = estate;
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}



