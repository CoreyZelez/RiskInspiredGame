#include "PlayerDiplomacy.h"
#include "Player.h"
#include <assert.h>
#include <iostream>

PlayerDiplomacy::PlayerDiplomacy(Player &player)
	: player(player)
{
}

void PlayerDiplomacy::update()
{
	attackHistory[maxHistory].clear();
	// Shift attack history keys right.
	for(int i = maxHistory; i >= 0; --i)
	{
		attackHistory[i + 1] = attackHistory[i];
	}
}

void PlayerDiplomacy::addAttackHistory(Player &enemy)
{
	assert(!sameUpperRealm(&player, &enemy));
	for(int i = 0; i <= 10; ++i)
	{
		if(attackHistory[i].count(&player) == 1)
		{
			attackHistory[i].erase(&player);
			break;
		}
	}
	attackHistory[0].insert(&player);
}
