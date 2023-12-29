#pragma once
#include "PlayerAIComponent.h"
#include "PlayerAIContext.h"

class LandArmy;
class NavalFleet;

class SimplePlayerAI : public PlayerAIComponent
{
public:
	SimplePlayerAI(Game &game, Player &player);

	virtual void handleTurn();

private:
	PlayerAIContext context;

	void handleTurnVassal();
	void handleTurnNonVassal();

	// Determines the strategic value of the territory for offensive or defensive purposes.
	int calculateArmyStrategicValue(const Territory &territory);
	int calculateFleetStrategicValue(const Territory &territory);
	void executeArmyAttacks(const std::vector<Territory*> &borderTerritories);
	void executeArmyAttack(LandArmy &army);
	void executeArmyMoveOrders(const std::map<Territory*, int> &strategicValues);
	void executeFleetAttacks(const std::vector<Territory*> &borderTerritories);
	void executeFleetAttack(NavalFleet &fleet);
	void executeFleetMoveOrders(const std::map<Territory*, int> &strategicValues);

};

