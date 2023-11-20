#include "PlayerAIContext.h"
#include "Player.h"
#include "Game.h"
#include "Territory.h"
#include <queue>
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
		const std::set<Territory*> &adjacencies = territory->getDistanceMap().getAdjacencies();
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
	const std::set<Territory*>& adjacencies = territory.getDistanceMap().getAdjacencies();
	std::set<Territory*> enemyAdjacencies;

	// Adds adjacent territories to enemyAdjacencies owned by enemy players.
	for(std::set<Territory*>::const_iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if((*iter)->getEstateOwner() != &player 
			&& (*iter)->getEstateOwner() != nullptr)
		{
			enemyAdjacencies.insert(*iter);
		}
	}

	return enemyAdjacencies;
}

std::map<const Player*, int> PlayerAIContext::getWeightedThreats(const Territory &territory)
{
	const std::set<Territory*>& adjacencies = territory.getDistanceMap().getAdjacencies();
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
	const int distanceFactor = 1.5;  // Calculation uses squared distance.
	for(std::set<const Player*>::const_iterator iter = enemyPlayers.begin(); 
		iter != enemyPlayers.end(); ++iter)
	{
		weightedThreats[*iter] = calculateWeightedThreat(territory, **iter, distanceFactor);
	}

	return weightedThreats;
}

std::map<std::pair<const Territory*, int>, std::vector<LandArmy*>> PlayerAIContext::getArmyBorderDistances(int maxDist)
{
	std::map<std::pair<const Territory*, int>, std::vector<LandArmy*>> armyBorderDistances;
	std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();
	for(const auto &army : armies)
	{
		Territory &armyTerritory = army.get()->getTerritory();
		for(const Territory* borderTerritory : getBorderTerritories())
		{
			assert(armyTerritory.getEstateOwner() == borderTerritory->getEstateOwner());
			const int distance = calculateFriendlyDistanceBFS(armyTerritory, *borderTerritory, maxDist);
			const std::pair<const Territory*, int> key = { borderTerritory, distance };
			armyBorderDistances[key].push_back(army.get());  // Holds underlying pointer of unique pointer. Be careful.
		}
	}
	return armyBorderDistances;
}

int calculateFriendlyDistanceBFS(const Territory &territory1, const Territory &territory2, int maxDist) {
	// Ensure territories have the same owner
	assert(territory1.getEstateOwner() == territory2.getEstateOwner());

	// Create a queue for BFS.
	std::queue<const Territory*> bfsQueue;

	// Create a set to keep track of visited territories
	std::unordered_set<const Territory*> visited;

	// Enqueue the starting territory.
	bfsQueue.push(&territory1);
	visited.insert(&territory1);

	// Start BFS
	int distance = 0;
	while(!bfsQueue.empty()) {
		// Process all territories at the current distance
		int currentQueueSize = bfsQueue.size();
		for(int i = 0; i < currentQueueSize; ++i) {
			const Territory* currentTerritory = bfsQueue.front();
			bfsQueue.pop();

			// Check if we reached territory2
			if(currentTerritory == &territory2) {
				return distance;
			}

			// Enqueue adjacent territories with the same owner
			for(const Territory* neighbor : currentTerritory->getDistanceMap().getAdjacencies()) {
				if(visited.find(neighbor) == visited.end() && neighbor->getEstateOwner() == territory1.getEstateOwner()) {
					bfsQueue.push(neighbor);
					visited.insert(neighbor);
				}
			}
		}

		// Move to the next distance level
		++distance;
		if(distance > maxDist)
		{
			break;
		}
	}

	// If territory2 is not reachable from territory1, return -1 or some special value
	return INT_MAX;
}

int calculateWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor)
{
	int threatScore = 0;
	const std::vector<std::unique_ptr<LandArmy>>& armies = player.getMilitaryManager().getArmies();
	for(const auto &army : armies)
	{
		const Territory &armyTerritory = army.get()->getTerritory();
		const int distance = territory.getDistanceMap().getDistance(armyTerritory);
		assert(distance > 0 || army.get()->isDead());
		if(distance > 0)
		{
			threatScore += army.get()->getTotalStrength() / pow(distance, distanceFactor);
		}
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

