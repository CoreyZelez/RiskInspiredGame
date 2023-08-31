#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Estate.h"
#include <assert.h>
#include <iostream>

Player::Player(Game& game, AIPersonality personality)
	: AIComponent(game, *this, personality), game(game), realm(*this)
{
}

void Player::handleTurn()
{
	militaryManager.removeDeadMilitaries();
	realm.handleFiefYields();

	if(!isHuman)
	{
		AIComponent.handleTurn();
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
