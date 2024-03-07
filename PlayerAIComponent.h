#pragma once
#include "AIPersonality.h"
#include <vector>

class Player;
class Game;
class Territory;

class PlayerAIComponent
{
public:
	PlayerAIComponent(Game &game, Player &player);

	virtual void handleTurn() = 0;

protected:
	Player &getPlayer();
	Game &getGame();

private:
	Game &game;
	Player &player;
};

