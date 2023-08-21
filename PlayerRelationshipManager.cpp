#include "PlayerRelationshipManager.h"
#include "Player.h"
#include <assert.h>

PlayerRelationshipManager::PlayerRelationshipManager(const Player &player)
	: player(player)
{
}

void PlayerRelationshipManager::addVassal(Player &vassal)
{
	assert(!vassal.getRelationshipManager().hasLiege());
	vassal.getRelationshipManager().liege = &(this->player);
	vassals.emplace_back(&vassal);
}

bool PlayerRelationshipManager::isVassal(const Player &player) const
{
	return liege == &player;
}

bool PlayerRelationshipManager::hasLiege() const
{
	return liege != nullptr;
}
