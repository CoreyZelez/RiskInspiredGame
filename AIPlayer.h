#pragma once
#include "Player.h"

class AIPlayer : public Player
{
public:
	AIPlayer(Game &game, AIPersonality personality);

	virtual void handleTurn() override;

private:
	AIPersonality personality;
};

