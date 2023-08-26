#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Estate.h"
#include <assert.h>
#include <iostream>

Player::Player(Game &game)
	: game(game), realm(*this)
{
}

void Player::handleTurn()
{
	if(awaitingTurnEnd)
	{
		return;
	}

	realm.handleFiefYields();
	// Specifies waiting for human player to end turn through user input.
	// TEMPORARILY TRUE FOR TESTING!!!
	awaitingTurnEnd = false;   
}

bool Player::getTurnOver() const
{
	return !awaitingTurnEnd;
}

void Player::completeTurn()
{
	assert(awaitingTurnEnd);
	// awaitingTurnEnd = false;
	awaitingTurnEnd = true;  // TEMPORARY FOR TESTING!!!
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
