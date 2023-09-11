#include "SimplePlayerAI.h"
#include "Player.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <iostream>

SimplePlayerAI::SimplePlayerAI(Game &game, Player &player)
	: PlayerAIComponent(game, player), context(player, game)
{
}

void SimplePlayerAI::handleTurn()
{
	Player &player = getPlayer();
	Game &game = getGame();

	// Calculate strategic values of friendly border territories.
	std::vector<Territory*> borderTerritories = context.getBorderTerritories();
	std::map<Territory*, int> strategicValues;
	for(Territory* territory : borderTerritories)
	{
		strategicValues[territory] = calculateStrategicValue(*territory);
	}

	executeArmyAttacks(borderTerritories);
}

int SimplePlayerAI::calculateStrategicValue(const Territory &territory) 
{
	Player &player = getPlayer();
	Game &game = getGame();

	// Calculate max and total threat of territory.
	std::map<const Player*, int> weightedThreats = context.getWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);  
	int totalThreat = calculateTotalThreat(weightedThreats);  

	// Calculate strategic value from enemy neighbouring territories.
	const std::set<Territory*> adjacencies = territory.getAdjacencies();
	std::set<const Territory*> enemyTerritories;
	// Determine enemy territories from adjacencies.
	for(std::set<Territory*>::iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if((*iter)->getEstateOwner() != &player)
		{
			enemyTerritories.insert(*iter);
		}
	}

	return 0;
}

void SimplePlayerAI::executeArmyAttacks(const std::vector<Territory*> &borderTerritories)
{
	Player &player = getPlayer();

	// Iterate over border territories and execute attacks by armies stationed at these territories.
	for(const Territory *territory : borderTerritories)
	{
		assert(territory->getEstateOwner() == &player);

		LandArmy *army = player.getMilitaryManager().getArmy(territory);
		if(army != nullptr)
		{
			executeArmyAttack(*army);
		}
	}
}

void SimplePlayerAI::executeArmyAttack(LandArmy &army)
{
	int strength = army.getStrength();
	Territory &territory = army.getTerritory();
	const std::set<Territory*> enemyTerritories = context.getEnemyAdjacencies(territory);

	for(std::set<Territory*>::iterator iter = enemyTerritories.begin();
		iter != enemyTerritories.end(); ++iter)
	{
		// Handle case when adjacent territory is a LandTerritory.
		LandTerritory* landTerritory = dynamic_cast<LandTerritory*>(*iter);
		if(landTerritory != nullptr)
		{
			const LandArmy *enemyArmy = landTerritory->getArmy();

			if(enemyArmy != nullptr)
			{
				//std::cout << enemyArmy->getStrength() << " " << strength << std::endl;
				//std::cout << "  " << (enemyArmy->getStrength() < 0.8 * strength) << std::endl;
				//std::cout << "  " << enemyArmy->getStrength() << " " << 0.8 * strength << std::endl;
			}

			if(enemyArmy == nullptr)
			{
				army.move(*landTerritory, strength);
			}
			else if(enemyArmy->getStrength() < 0.8 * strength)
			{
				army.move(*landTerritory, strength);
			}
		}

		// Update strength.
		strength = army.getStrength();
		// Return since army dead.
		if(strength == 0)
		{
			return;
		}
	}
}
