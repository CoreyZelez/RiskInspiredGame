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

NavalFleet::NavalFleet(Player & owner, Territory * location, std::array<unsigned int, 4> staminaStrength)
	: MilitaryForce(owner, location, staminaStrength, *(TextureManager::getInstance().getTexture("landArmy")))
{
	assert(location != nullptr);
}

void NavalFleet::move(Territory &location, unsigned int strength)
{
}

void NavalFleet::moveClosest(Territory & target, unsigned int strength, int maxDist)
{
}

std::pair<int, int> NavalFleet::calculateMinMaxStaminaCost(const Territory & territory) const
{
	// temp.
	return { 4, 4 };
}

