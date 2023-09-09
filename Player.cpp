#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "SimplePlayerAI.h"
#include "Estate.h"
#include <assert.h>
#include <iostream>

Player::Player(Game &game, AIPersonality personality)
	: game(game), realm(*this)
{
}

Player::Player(Game& game)
	: game(game), realm(*this), AIComponent(std::make_unique<SimplePlayerAI>(game, *this))
{
}

void Player::handleTurn()
{
	militaryManager.removeDeadMilitaries();
	realm.getEstateManager().handleFiefYields();

	if(!isHuman)
	{
		AIComponent.get()->handleTurn();
	}
}

void Player::setHuman()
{
	isHuman = true;
}

bool Player::getIsHuman() const
{
	return isHuman;
}

MilitaryManager& Player::getMilitaryManager()
{
	return militaryManager;
}

const MilitaryManager & Player::getMilitaryManager() const
{
	return militaryManager;
}

Realm& Player::getRealm()
{
	return realm;
}

const Realm& Player::getRealm() const
{
	return realm;
}
