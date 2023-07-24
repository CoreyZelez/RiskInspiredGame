#include "Territory.h"

bool Territory::attemptOccupy(std::shared_ptr<LandArmy> &army)
{
	return false;
}

bool Territory::attemptOccupy(std::shared_ptr<NavalFleet> &fleet)
{
	return false;
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}
