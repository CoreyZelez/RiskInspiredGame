#include "PlayerAIComponent.h"

PlayerAIComponent::PlayerAIComponent(Game &game, Player &player)
	: game(game), player(player)
{
}

Player& PlayerAIComponent::getPlayer()
{
	return player;
}

Game& PlayerAIComponent::getGame()
{
	return game;
}
