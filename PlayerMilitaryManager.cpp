#include "PlayerMilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

void PlayerMilitaryManager::addLandArmy(std::shared_ptr<LandArmy> army)
{
	assert(army.get()->getStrength() > 0);
	armies.push_back(army);
}

void PlayerMilitaryManager::addNavalFleet(std::shared_ptr<NavalFleet> fleet)
{
	assert(fleet.get()->getStrength() > 0);
	navies.push_back(fleet);
}


