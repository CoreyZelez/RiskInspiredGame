#include "NavalFleet.h"
#include "Territory.h"
#include "Player.h"
#include "TextureManager.h"
#include <assert.h>
#include <memory>

NavalFleet::NavalFleet(Player &owner, Territory *location, int strength)
	: MilitaryForce(owner, location, strength, *(TextureManager::getInstance().getTexture("landArmy")))
{
	assert(location != nullptr);
}

void NavalFleet::move(Territory &location, int strength)
{
}

void NavalFleet::moveClosest(Territory & target, int strength, int maxDist)
{
}

std::pair<int, int> NavalFleet::calculateMinMaxStaminaCost(const Territory & territory) const
{
	// temp.
	return { 4, 4 };
}

