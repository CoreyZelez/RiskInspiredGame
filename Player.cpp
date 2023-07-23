#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

void Player::addArmy(std::unique_ptr<LandArmy> army)
{
	assert(army.get()->getStrength() > 0);
	armies.emplace_back(std::move(army));
}

void Player::addNavy(std::unique_ptr<NavalFleet> navy)
{
	assert(navy.get()->getStrength() > 0);
	navies.emplace_back(std::move(navy));
}
