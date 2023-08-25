#include "MilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>
#include <iostream>

void MilitaryManager::addLandArmy(std::shared_ptr<LandArmy> army)
{
	assert(army.get() != nullptr);
	assert(army.get()->getStrength() > 0);
	armies.push_back(army);
}

void MilitaryManager::addNavalFleet(std::shared_ptr<NavalFleet> fleet)
{
	assert(fleet.get()->getStrength() > 0);
	navies.push_back(fleet);
}


