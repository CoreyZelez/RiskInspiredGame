#include "PlayerAIContext.h"
#include "Player.h"
#include "Game.h"
#include "Territory.h"
#include <set>
#include <iostream>

PlayerAIContext::PlayerAIContext(Player &player, Game &game)
	: player(player), game(game)
{
}

std::vector<Territory*> PlayerAIContext::getBorderTerritories()
{
	std::set<Territory*> &realmTerritories = player.getRealm().getEstateManager().getTerritories();
	std::vector<Territory*> borderTerritories = {};

	for(Territory* territory : realmTerritories)
	{
		assert(territory->getEstateOwner() == &player);

		// Add territory to border territories if one of its adjacencies is enemy owned.
		const std::set<Territory*> &adjacencies = territory->getAdjacencies();
		for(const auto &adjacency : adjacencies)
		{
			// Checks whether adjacent territory is apart of realms territories.
			if(realmTerritories.count(const_cast<Territory*>(adjacency)) == 0)
			{
				// Territory is border territory since an adjacent territory is apart of enemy player realm.
				borderTerritories.push_back(territory);
				break;
			}
		}
	}

	return borderTerritories;
}

const std::set<Territory*> PlayerAIContext::getEnemyAdjacencies(Territory &territory)
{
	const std::set<Territory*>& adjacencies = territory.getAdjacencies();
	std::set<Territory*> enemyAdjacencies;

	// Adds adjacent territories to enemyAdjacencies owned by enemy players.
	for(std::set<Territory*>::const_iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if((*iter)->getEstateOwner() != &player)
		{
			enemyAdjacencies.insert(*iter);
		}
	}

	return enemyAdjacencies;
}

std::map<const Player*, int> PlayerAIContext::getWeightedThreats(const Territory &territory)
{
	const std::set<Territory*>& adjacencies = territory.getAdjacencies();
	std::set<const Player*> enemyPlayers = {};
	std::map<const Player*, int> weightedThreats;

	// Determines enemy players holding adjacent territories.
	for(std::set<Territory*>::iterator iter = adjacencies.begin(); 
		iter != adjacencies.end(); ++iter)
	{
		const Player *owner = (*iter)->getEstateOwner(); 
		if(owner != nullptr && owner != &player)
		{
			enemyPlayers.insert(owner);
		}
	}

	// Calculated weighted threats of each enemy player.
	const int distanceFactor = 2;  // Calculation uses squared distance.
	for(std::set<const Player*>::const_iterator iter = enemyPlayers.begin(); 
		iter != enemyPlayers.end(); ++iter)
	{
		weightedThreats[*iter] = calculateWeightedThreat(territory, **iter, distanceFactor);
	}

	return weightedThreats;
}

int calculateWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor)
{
	int threatScore = 0;
	const std::vector<std::unique_ptr<LandArmy>>& armies = player.getMilitaryManager().getArmies();
	for(const auto &army : armies)
	{
		const Territory &armyTerritory = army.get()->getTerritory();
		const int distance = territory.getDistance(armyTerritory);
		assert(distance > 0);
		threatScore += army.get()->getStrength() / pow(distance, distanceFactor);
	}
	return threatScore;
}

int calculateMaxThreat(const std::map<const Player*, int>& threats)
{
	int maxThreat = 0; // Initialize to the smallest possible integer value
	for(const auto& pair : threats)
	{
		if(pair.second > maxThreat)
		{
			maxThreat = pair.second;
		}
	}
	return maxThreat;
}

int calculateTotalThreat(const std::map<const Player*, int>& threats)
{
	int totalThreat = 0;

	for(const auto& pair : threats)
	{
		totalThreat += pair.second;
	}

	return totalThreat;
}
