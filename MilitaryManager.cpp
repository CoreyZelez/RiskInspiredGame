#include "MilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

void MilitaryManager::addArmy(std::shared_ptr<LandArmy> army)
{
	assert(army.get()->getStrength() > 0);
	armies.push_back(std::move(army));
}

void MilitaryManager::addNavy(std::shared_ptr<NavalFleet> navy)
{
	assert(navy.get()->getStrength() > 0);
	navies.push_back(std::move(navy));
}


