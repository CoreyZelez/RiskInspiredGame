#include "PlayerAIContext.h"
#include "Player.h"
#include "Game.h"
#include "Territory.h"
#include <queue>
#include <set>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

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

const std::set<Territory*> PlayerAIContext::getEnemyAdjacencies(Territory &territory, bool includeNeutral)
{
	const std::set<Territory*>& adjacencies = territory.getDistanceMap().getAdjacencies();
	std::set<Territory*> enemyAdjacencies;

	// Adds adjacent territories to enemyAdjacencies owned by enemy players.
	for(std::set<Territory*>::const_iterator iter = adjacencies.begin();
		iter != adjacencies.end(); ++iter)
	{
		if(includeNeutral && (*iter)->getEstateOwner() != &player)
		{
			enemyAdjacencies.insert(*iter);
		}
		else if((*iter)->getEstateOwner() != &player && (*iter)->getEstateOwner() != nullptr)
		{
			enemyAdjacencies.insert(*iter);
		}
	}

	return enemyAdjacencies;
}

std::map<const Player*, int> PlayerAIContext::getArmyWeightedThreats(const Territory &territory)
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
		weightedThreats[*iter] = calculateArmyWeightedThreat(territory, **iter, distanceFactor);
	}

	return weightedThreats;
}

std::map<const Player*, int> PlayerAIContext::getFleetWeightedThreats(const Territory & territory)
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
		weightedThreats[*iter] = calculateFleetWeightedThreat(territory, **iter, distanceFactor);
	}

	return weightedThreats;
}

std::unordered_map<std::pair<const Territory*, int>, std::vector<LandArmy*>, PairTerritoryIntHash> PlayerAIContext::getArmyBorderDistances(int maxDist)
{
	////std::unordered_map<std::pair<const Territory*, int>, std::vector<LandArmy*>, PairTerritoryIntHash> armyBorderDistances;
	////std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();
	////const std::vector<Territory*> borderTerritoriesVec = getBorderTerritories();
	////const std::unordered_set<const Territory*> borderTerritories(borderTerritoriesVec.begin(), borderTerritoriesVec.end());
	////for(const auto &army : armies)
	////{
	////	Territory &armyTerritory = army.get()->getTerritory();
	////	std::unordered_map<const Territory*, int> distances = calculateFriendlyDistancesBFS(armyTerritory, borderTerritories, maxDist);
	////	// Add army to armyBorderDistances.
	////	for(const Territory *territory : borderTerritories)
	////	{
	////		std::pair<const Territory*, int> key(territory, distances[territory]);
	////		armyBorderDistances[key].push_back(army.get());
	////	}
	////}
	////return armyBorderDistances;

	std::mutex mutex;  // Declare a mutex for thread safety.
	std::vector<std::thread> threads;  // Store threads for joining later
	
	std::unordered_map<std::pair<const Territory*, int>, std::vector<LandArmy*>, PairTerritoryIntHash> armyBorderDistances;
	std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();
	const std::vector<Territory*> borderTerritoriesVec = getBorderTerritories();
	const std::unordered_set<const Territory*> borderTerritories(borderTerritoriesVec.begin(), borderTerritoriesVec.end());
	
	// Launch a thread for each army
	for(auto& army : armies)
	{
		threads.emplace_back(&PlayerAIContext::getIndividualArmyBorderDistances, this, std::ref(army), std::ref(armyBorderDistances), 
			std::cref(borderTerritories), maxDist, std::ref(mutex));
	}
	
	// Wait for all threads to finish
	for(auto& thread : threads)
	{
		thread.join();
	}
	
	return armyBorderDistances;
}

void PlayerAIContext::getIndividualArmyBorderDistances(std::unique_ptr<LandArmy>& army, std::unordered_map<std::pair<const Territory*, int>,
	std::vector<LandArmy*>, PairTerritoryIntHash>& armyBorderDistances, const std::unordered_set<const Territory*>& borderTerritories, int maxDist, std::mutex &mutex)
{
	Territory &armyTerritory = army->getTerritory();
	std::unordered_map<const Territory*, int> distances = calculateFriendlyDistancesBFS(armyTerritory, borderTerritories, maxDist);

	// Add army to armyBorderDistances.
	for(const Territory *territory : borderTerritories)
	{
		std::pair<const Territory*, int> key(territory, distances[territory]);
		// Use a mutex to protect the shared data structure.
		std::lock_guard<std::mutex> lock(mutex);
		armyBorderDistances[key].push_back(army.get());
	}
}

void PlayerAIContext::getIndividualFleetBorderDistances(std::unique_ptr<NavalFleet>& fleet, std::unordered_map<std::pair<const Territory*, int>, 
	std::vector<NavalFleet*>, PairTerritoryIntHash>& fleetBorderDistances, const std::unordered_set<const Territory*>& borderTerritories, int maxDist, std::mutex &mutex)
{
	Territory &fleetTerritory = fleet.get()->getTerritory();
	std::unordered_map<const Territory*, int> distances = calculateFriendlyDistancesBFS(fleetTerritory, borderTerritories, maxDist);
	// Add army to armyBorderDistances.
	for(const Territory *territory : borderTerritories)
	{
		assert(fleetTerritory.getEstateOwner() == territory->getEstateOwner());
		std::pair<const Territory*, int> key(territory, distances[territory]);
		std::lock_guard<std::mutex> lock(mutex);
		fleetBorderDistances[key].push_back(fleet.get());
	}
}

std::unordered_map<std::pair<const Territory*, int>, std::vector<NavalFleet*>, PairTerritoryIntHash> PlayerAIContext::getFleetBorderDistances(int maxDist)
{
	
	
	//WARNING! RETURNS DISTANCES TO LAND TERRITORIES AS WELL!!! SHOULD PROBALY EXCLUDE I WOULD IMAGINE!!!!!!!!!!
	
	
	////std::unordered_map<std::pair<const Territory*, int>, std::vector<NavalFleet*>, PairTerritoryIntHash> fleetBorderDistances;
	////std::vector<std::unique_ptr<NavalFleet>> &fleets = player.getMilitaryManager().getFleets();
	////const std::vector<Territory*> borderTerritoriesVec = getBorderTerritories();
	////const std::unordered_set<const Territory*> borderTerritories(borderTerritoriesVec.begin(), borderTerritoriesVec.end());
	////for(const auto &fleet : fleets)
	////{
	////	Territory &fleetTerritory = fleet.get()->getTerritory();
	////	std::unordered_map<const Territory*, int> distances = calculateFriendlyDistancesBFS(fleetTerritory, borderTerritories, maxDist);
	////	// Add army to armyBorderDistances.
	////	for(const Territory *territory : borderTerritories)
	////	{
	////		assert(fleetTerritory.getEstateOwner() == territory->getEstateOwner());
	////		std::pair<const Territory*, int> key(territory, distances[territory]);
	////		fleetBorderDistances[key].push_back(fleet.get());
	////	}
	////}
	////return fleetBorderDistances;

	std::mutex mutex;  // Declare a mutex for thread safety.
	std::vector<std::thread> threads;  // Store threads for joining later.
	
	std::unordered_map<std::pair<const Territory*, int>, std::vector<NavalFleet*>, PairTerritoryIntHash> fleetBorderDistances;
	std::vector<std::unique_ptr<NavalFleet>> &fleets = player.getMilitaryManager().getFleets();
	const std::vector<Territory*> borderTerritoriesVec = getBorderTerritories();
	const std::unordered_set<const Territory*> borderTerritories(borderTerritoriesVec.begin(), borderTerritoriesVec.end());
	
	// Launch a thread for each army
	for(auto &fleet : fleets)
	{
		threads.emplace_back(&PlayerAIContext::getIndividualFleetBorderDistances, this, std::ref(fleet), std::ref(fleetBorderDistances),
			std::cref(borderTerritories), maxDist, std::ref(mutex));
	}
	
	// Wait for all threads to finish
	for(auto& thread : threads)
	{
		thread.join();
	}
	
	return fleetBorderDistances;
}

int calculateFriendlyDistanceBFS(const Territory &territory1, const Territory &territory2, int maxDist) 
{
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

std::unordered_map<const Territory*, int> calculateFriendlyDistancesBFS(const Territory &sourceTerritory, const std::unordered_set<const Territory*>& territories, int maxDist)
{
	std::unordered_map<const Territory*, int> distances;

	// Create a queue for BFS.
	std::queue<const Territory*> bfsQueue;

	// Create a set to keep track of visited territories.
	std::unordered_set<const Territory*> visited;

	// Enqueue the starting territory.
	bfsQueue.push(&sourceTerritory);
	visited.insert(&sourceTerritory);

	// Perform BFS until maxDist exceeded or distance for every required territory is found.
	int distance = 0;
	while(!bfsQueue.empty() && (territories.size() > distances.size())) 
	{
		// Process all territories at the current distance.
		int currentQueueSize = bfsQueue.size();
		for(int i = 0; i < currentQueueSize; ++i) {
			const Territory* currentTerritory = bfsQueue.front();
			bfsQueue.pop();

			// Check if we reached territory2.
			if(territories.count(currentTerritory) == 1 && distances.count(currentTerritory) == 0) 
			{
				distances[currentTerritory] = distance;
			}

			// Enqueue adjacent territories with the same owner.
			for(const Territory* neighbor : currentTerritory->getDistanceMap().getAdjacencies()) {
				if(visited.find(neighbor) == visited.end() && neighbor->getEstateOwner() == sourceTerritory.getEstateOwner()) {
					bfsQueue.push(neighbor);
					visited.insert(neighbor);
				}
			}
		}

		// Move to the next distance level.
		++distance;
		if(distance > maxDist)
		{
			break;
		}
	}

	// Set INT_MAX for territories which were not traversed as they exceed maxDist.
	for(const auto &territory : territories)
	{
		if(distances.count(territory) == 0)
		{
			distances[territory] = INT_MAX;
		}
	}

	return distances;
}

int calculateArmyWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor)
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

int calculateFleetWeightedThreat(const Territory & territory, const Player & player, const float distanceFactor)
{
	int threatScore = 0;
	const std::vector<std::unique_ptr<NavalFleet>>& fleets = player.getMilitaryManager().getFleets();
	for(const auto &fleet : fleets)
	{
		const Territory &armyTerritory = fleet.get()->getTerritory();
		const int distance = territory.getDistanceMap().getDistance(armyTerritory);
		assert(distance > 0 || army.get()->isDead());
		if(distance > 0)
		{
			threatScore += fleet.get()->getTotalStrength() / pow(distance, distanceFactor);
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

