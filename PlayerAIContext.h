#pragma once
#include <vector>

class Player;
class Game;
class Territory;

/* Provides information to be used for AI decision making.*/
class PlayerAIContext
{
public:
	PlayerAIContext(Player &player, Game &game);

	std::vector<Territory*> getBorderTerritories();

private:
	Player &player;
	Game &game;
};

