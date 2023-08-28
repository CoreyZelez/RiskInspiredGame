#pragma once
#include "AIPersonality.h"

class Player;
class Game;


class AIPlayerComponent
{
public:
	AIPlayerComponent(Game &game, Player &player, AIPersonality personality);

	void handleTurn();

private:
	Game &game;
	Player &player;
	AIPersonality personality;
};

