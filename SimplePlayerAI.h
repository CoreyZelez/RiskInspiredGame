#pragma once
#include "PlayerAIComponent.h"

class SimplePlayerAI : public PlayerAIComponent
{
public:
	SimplePlayerAI(Game &game, Player &player);

	virtual void handleTurn();
};

