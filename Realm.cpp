#include "Realm.h"
#include "Player.h"
#include "Estate.h"
#include "Utility.h"
#include <assert.h>
#include <iostream>

Realm::Realm(Player &player)
	: player(player), militaryManager(player.getMilitaryManager())
{
	grid.setColor(createRandomRealmColor());
}

void Realm::draw(sf::RenderWindow & window) const
{
	if(drawVassalRealms)
	{
		for(Player *vassal : vassals)
		{
			vassal->getRealm().draw(window);
		}
	}
	else
	{
		grid.draw(window);
	}
}

void Realm::handleFiefYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(auto &fief : fiefs)
	{
		fief->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units).
	for(auto &fief : fiefs)
	{
		fief->yield(militaryManager);
	}
}

void Realm::addFief(Estate *fief)
{
	fiefs.emplace_back(fief);
	assert(fief->compareRuler(&this->player));
	grid.addGrid(fief->getGrid());
}

void Realm::removeFief(const Estate *fief)
{
	for(auto iter = fiefs.begin(); iter != fiefs.end(); ++iter)
	{
		if(*iter == fief)
		{
			fiefs.erase(iter);
			return;
		}
	}
	assert(false);  // Functions should only be called when the estate owner is this player.
}

void Realm::addVassal(Player &vassal)
{
	assert(!vassal.getRealm().hasLiege());
	vassal.getRealm().liege = &(this->player);
	vassals.emplace_back(&vassal);
}

bool Realm::isVassal(const Player &player) const
{
	return liege == &player;
}

bool Realm::hasLiege() const
{
	return liege != nullptr;
}
