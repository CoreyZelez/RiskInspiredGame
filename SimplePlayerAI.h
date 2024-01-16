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

	// Determines the strategic value of the territory for offensive or defensive purposes of fleets.
	int calculateArmyStrategicValue(const Territory &territory);
	// Determines the strategic value of the territory for offensive or defensive purposes of fleets.
	int calculateFleetStrategicValue(const Territory &territory);

	// Executes attacks of all armies.
	void executeArmyAttacks(const std::vector<Territory*> &borderTerritories);
	// Executes army attack of a single army.
	void executeArmyAttack(LandArmy &army);
	// Executes move orders of all armies.
	void executeArmyMoveOrders(const std::map<Territory*, int> &strategicValues);

	// Executes attacks of all fleets.
	void executeFleetAttacks(const std::vector<Territory*> &borderTerritories);
	// Executes attack of a single fleet.
	void executeFleetAttack(NavalFleet &fleet);
	// Executes move orders of all fleets.
	void executeFleetMoveOrders(const std::map<Territory*, int> &strategicValues);

	// Handles decision to rebel against liege. Returns true if rebellion is executed.
	bool executeRebellionAgainstLiege();
};

