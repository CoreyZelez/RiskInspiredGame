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
	if(awaitingUserInput)
	{
		return;
	}

	realm.handleFiefYields();
	// Specifies waiting for human player to end turn through user input.
	awaitingUserInput = true;
}

bool Player::getAwaitingUserInput() const
{
	return !awaitingUserInput;
}

void Player::completeTurn()
{
	assert(awaitingUserInput);
	awaitingUserInput = false;
}

MilitaryManager& Player::getMilitaryMangager()
{
	return militaryManager;
}

Realm& Player::getRealm()
{
	return realm;
}