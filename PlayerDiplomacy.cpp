#include "PlayerDiplomacy.h"
#include "Player.h"
#include <assert.h>
#include <iostream>

PlayerDiplomacy::PlayerDiplomacy(Player &player)
	: player(player)
{
}

PlayerDiplomacy::~PlayerDiplomacy()
{
	// Remove diplomacy of this player from other diplomacy objects.
	for(int i = 0; i <= maxHistory; ++i)
	{
		for(Player* player : attackHistory[i])
		{
			player->removeDiplomacyWithPlayer(this->player);
		}
	}
}

void PlayerDiplomacy::update()
{
	// Remove rebelling vassals at end of attack history.
	for(Player *player : attackHistory[maxHistory])
	{
		rebellingVassals.erase(player);
	}

	// Shift attack history keys right.
	for(int i = maxHistory; i >= 1; --i)
	{
		attackHistory[i] = attackHistory[i - 1];
	}
	attackHistory[0].clear();
}

void PlayerDiplomacy::setColors(const std::vector<std::unique_ptr<Player>> &players)
{
	std::unordered_set<const Player*> playerColorsSet;

	player.getRealm().setGridColor(sf::Color(0, 94, 255));
	playerColorsSet.insert(&player);

	// Color of realms recently attacked. Closer to yellow implies longer time since attack.
	sf::Color warColor(255, 0, 0);
	sf::Color rebelColor(255, 0, 100);

	const int greenIncrement = 255 / (maxHistory + 1);
	for(int i = 0; i <= maxHistory; ++i)
	{
		if(i > 0)
		{
			warColor.g += greenIncrement;
		}
		for(Player *player : attackHistory[i])
		{
			if(rebellingVassals.count(player) == 1)
			{
				player->getRealm().setGridColor(rebelColor);
			}
			else
			{
				player->getRealm().setGridColor(warColor);
			}
			playerColorsSet.insert(player);
		}
	}

	// Set all remaining realm colors to grey.
	const sf::Color grey(50, 50, 50);
	for(auto &player : players)
	{
		if(playerColorsSet.count(player.get()) == 0)
		{
			player.get()->getRealm().setGridColor(grey);
			playerColorsSet.insert(player.get());
		}
	}
}

void PlayerDiplomacy::addAttackHistory(Player &enemy)
{
	assert(!sameUpperRealm(&player, &enemy));
	assert(&player != &enemy);
	for(int i = 0; i <= maxHistory; ++i)
	{
		if(attackHistory[i].count(&enemy) == 1)
		{
			attackHistory[i].erase(&enemy);
			break;
		}
	}
	attackHistory[0].insert(&enemy);
}

void PlayerDiplomacy::addRebellingVassal(Player &rebellingVassal)
{
	rebellingVassal.addAttackHistory(player);
	addAttackHistory(rebellingVassal);
	rebellingVassals.insert(&rebellingVassal);
}

void PlayerDiplomacy::removeDiplomacyWithPlayer(Player &player)
{
	assert(&player != &this->player);
	for(int i = 0; i <= maxHistory; ++i)
	{
		attackHistory[i].erase(&player);
	}
	rebellingVassals.erase(&player);
}
