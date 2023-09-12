#pragma once
#include "PlayerAIComponent.h"
#include "PlayerAIContext.h"

class LandArmy;

class SimplePlayerAI : public PlayerAIComponent
{
public:
	SimplePlayerAI(Game &game, Player &player);

	virtual void handleTurn();

private:
	PlayerAIContext context;

	// Determines the strategic value of the territory for offensive or defensive purposes.
	int calculateStrategicValue(const Territory &territory);
	void executeArmyAttacks(const std::vector<Territory*> &borderTerritories);
	void executeArmyAttack(LandArmy &army);
	void executeMoveOrders(std::map<Territory*, int> strategicValues);
};

