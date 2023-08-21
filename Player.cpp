#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Estate.h"
#include <assert.h>
#include <iostream>

Player::Player()
	: relationshipManager(*this)
{
}

void Player::handleFiefYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(auto &fief : fiefs)
	{
		fief.get()->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units).
	for(auto &fief : fiefs)
	{
		fief.get()->yield(militaryManager);
	}
}

void Player::addFief(std::shared_ptr<Estate> estate)
{
	fiefs.emplace_back(estate);
	assert(estate.get()->compareRuler(this));
}

void Player::removeFief(const Estate *fief)
{
	for(auto iter = fiefs.begin(); iter != fiefs.end(); ++iter)
	{
		if(iter->get() == fief)
		{
			fiefs.erase(iter);
			return;
		}
	}
	assert(false);  // Functions should only be called when the estate owner is this player.
}

PlayerRelationshipManager & Player::getRelationshipManager()
{
	return relationshipManager;
}


