#pragma once
#include "AIPersonality.h"

class Player;
class Game;


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

