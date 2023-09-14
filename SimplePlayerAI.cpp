#include "SimplePlayerAI.h"
#include "Player.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <iostream>
#include <algorithm>

SimplePlayerAI::SimplePlayerAI(Game &game, Player &player)
	: PlayerAIComponent(game, player), context(player, game)
{
}

void SimplePlayerAI::handleTurn()
{
	Player &player = getPlayer();
	Game &game = getGame();

	// Calculate border territories prior to any attacks.
	std::vector<Territory*> borderTerritories = context.getBorderTerritories();
	executeArmyAttacks(borderTerritories);

	// Calculate strategic values of friendly border territories.
	// Recalculate border territories post attack executions.
	borderTerritories = context.getBorderTerritories();
	std::map<Territory*, int> strategicValues;
	for(Territory* territory : borderTerritories)
	{
		assert(calculateStrategicValue(*territory) >= 0);
		strategicValues[territory] = calculateStrategicValue(*territory);
	}
	executeLandMoveOrders(strategicValues);
}

int SimplePlayerAI::calculateStrategicValue(const Territory &territory) 
{
	Player &player = getPlayer();
	Game &game = getGame();

	int strategicValue = 0;

	// Calculate strategic value from threat of attack of territory.
	std::map<const Player*, int> weightedThreats = context.getWeightedThreats(territory);
	int maxThreat = calculateMaxThreat(weightedThreats);  
	int totalThreat = calculateTotalThreat(weightedThreats);  
	strategicValue += maxThreat + totalThreat;

	// Calculate strategic desire to attack enemy neighbouring territories.
	const std::set<Territory*> adjacencies = territory.getAdjacencies(false);
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

	assert(strategicValue >= 0);

	return strategicValue;
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

			if(enemyArmy == nullptr)
			{
				int attackStrength = 0.3 * strength;

				if(attackStrength <= 1)
				{
					attackStrength = strength;
				}
				army.move(*landTerritory, attackStrength);
			}
			else if(enemyArmy->getStrength() < 0.1 * strength)
			{
				int attackStrength = 0.4 * strength;
				if(attackStrength == 0)
				{
					attackStrength = 1;
				}
				army.move(*landTerritory, attackStrength);
			}
			else if(enemyArmy->getStrength() < 0.6 * strength)
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


void SimplePlayerAI::executeLandMoveOrders(const std::map<Territory*, int> &strategicValues)
{
	Player &player = getPlayer();
	std::map<Territory*, int> remainingStrategicValues = strategicValues;

	// Find sum of strategic values.
	int totalStrategicValue = 0; 
	for(const auto& pair : strategicValues)
	{
		assert(pair.second >= 0);
		totalStrategicValue += pair.second;
	}

	// Total strength of player's land military.
	const int totalArmyStrength = player.getMilitaryManager().getTotalArmyStrength();
	assert(totalArmyStrength >= 0);
	if(totalArmyStrength == 0)
	{
		return;
	}

	// Strategic value that is allocated to each unit of army strength.
	float strategicValuePerUnit = (float)totalStrategicValue / (float)totalArmyStrength;
	strategicValuePerUnit; // testing.
	assert(strategicValuePerUnit >= 0);

	std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();

	// Keeps track of what amount of strength of a given army is free to move from
	// its current territory. 
	std::map<LandArmy*, int> freeToMove;
	for(auto &army : armies)
	{
		freeToMove[army.get()] = army.get()->getStrength();
	}

	// Armies are added through movement so only iterate up to pre movement number of armies. 
	const int numArmies = armies.size(); 
	assert(numArmies > 0);

	// Partially allocates armies prioritising closest first. Closer armies provide greater allocation.
	for(int distance = 0; distance <= 13; ++distance)
	{
		for(auto& pair : remainingStrategicValues)
		{
			Territory& territory = *pair.first;
			int &strategicValue = pair.second;

			assert(strategicValue >= 0);

			if(strategicValue == 0)
			{
				continue;
			}

			// Move armies of specified distance towards the territory.
			for(int i = 0; i < numArmies; ++i)
			{

				LandArmy &army = *armies[i].get();

				const Territory &armyTerritory = army.getTerritory();
				const int armyDistance = territory.getDistance(armyTerritory, true);

				assert(remainingStrategicValues.count(&territory) == 1);
				assert(strategicValue > 0);
				assert(freeToMove[&army] <= army.getStrength());

				if(armyDistance != distance)
				{
					continue;
				}
				else if(freeToMove[&army] == 0)
				{
					continue;
				}

				// Move army to friendly territory closest to territory.
				// In future prioritise border territories!!!
				const float moveRatio = 0.7;
				int moveStrength = freeToMove[&army] * moveRatio;
				const int maximumAllocation = strategicValue / strategicValuePerUnit;
				// Move strength is capped by maximum allocation amount derived from territory priority.
				moveStrength = std::min(moveStrength, maximumAllocation);
				if(moveStrength != 0)
				{
					army.moveClosest(territory, moveStrength);
					freeToMove[&army] -= moveStrength;
					const int usedStrategicValue = 1 + moveStrength * strategicValuePerUnit;
					strategicValue -= usedStrategicValue;
					if(strategicValue <= 0)
					{
						strategicValue = 0;
						break;
					}
					assert(freeToMove[&army] <= army.getStrength());
				}
			}
		}
	}
}
