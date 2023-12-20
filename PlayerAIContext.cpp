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
	const std::unordered_set<Territory*> realmTerritories = player.getRealm().getTerritories();
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

std::unordered_map<const Territory*, std::unordered_map<int, std::vector<LandArmy*>>> PlayerAIContext::getArmyBorderDistances(int maxDist)
{
	std::mutex mutex;  // Declare a mutex for thread safety.
	std::vector<std::thread> threads;  // Store threads for joining later
	
	// minBuckets is chosen based upon observation of maximum buckets needed when running game.
	// In future may need to alter or make more dynamic
	const size_t minBuckets = 50;
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<LandArmy*>>> armyBorderDistances(minBuckets);
	const std::vector<Territory*> borderTerritories = getBorderTerritories();
	// Launch a thread for each territory to determine its distance to each army.
	for(const Territory *territory : borderTerritories)
	{
		threads.emplace_back(&PlayerAIContext::determineTerritoryArmyDistances, this, std::ref(*territory), std::ref(armyBorderDistances), maxDist, std::ref(mutex));
	}
	
	// Wait for all threads to finish
	for(auto& thread : threads)
	{
		thread.join();
	}

	return armyBorderDistances;
}

void PlayerAIContext::determineTerritoryArmyDistances(const Territory &territory, 
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<LandArmy*>>>& armyTerritoryDistances, int maxDist, std::mutex & mutex)
{
	// Holds distance of each army to territory.
	std::unordered_map<int, std::vector<LandArmy*>> armyDistances;
	const std::vector<std::unique_ptr<LandArmy>> &armies = player.getMilitaryManager().getArmies();
	std::unordered_set<const Territory*> armyTerritories;

	// Determine all army territories
	for(const auto &army : armies)
	{
		const Territory *armyTerritory = &army.get()->getTerritory();
		armyTerritories.insert(armyTerritory);
	}

	// Friendly distances of territory to set of territories occupied by friendly army.
	// We use this more complex procedure rather than calling calculateFriendlyDistanceBFS on each army territory individually since
	// it means we only have to run BFS once rather than once per army.
	std::unordered_map<const Territory*, int> territoryDistances = calculateFriendlyDistancesBFS(territory, armyTerritories, maxDist);

	// Determine distances to specific armies.
	for(const auto &army : armies)
	{
		const Territory *armyTerritory = &army.get()->getTerritory();
		const int distance = territoryDistances[armyTerritory];
		armyDistances[distance].push_back(army.get());
	}

	std::lock_guard<std::mutex> lock(mutex);
	armyTerritoryDistances[&territory] = armyDistances;
}

void PlayerAIContext::determineTerritoryFleetDistances(const Territory & territory, std::unordered_map<const Territory*, 
	std::unordered_map<int, std::vector<NavalFleet*>>>& fleetTerritoryDistances, int maxDist, std::mutex & mutex)
{
	// Holds distance of each army to territory.
	std::unordered_map<int, std::vector<NavalFleet*>> fleetDistances;
	const std::vector<std::unique_ptr<NavalFleet>> &fleets = player.getMilitaryManager().getFleets();
	std::unordered_set<const Territory*> fleetTerritories;

	// Determine all army territories
	for(const auto &fleet : fleets)
	{
		const Territory *armyTerritory = &fleet.get()->getTerritory();
		fleetTerritories.insert(armyTerritory);
	}

	// Friendly distances of territory to set of territories occupied by friendly army.
	// We use this more complex procedure rather than calling calculateFriendlyDistanceBFS on each army territory individually since
	// it means we only have to run BFS once rather than once per army.
	std::unordered_map<const Territory*, int> territoryDistances = calculateFriendlyDistancesBFS(territory, fleetTerritories, maxDist);

	// Determine distances to specific armies.
	for(const auto &fleet : fleets)
	{
		const Territory *fleetTerritory = &fleet.get()->getTerritory();
		const int distance = territoryDistances[fleetTerritory];
		fleetDistances[distance].push_back(fleet.get());
	}

	std::lock_guard<std::mutex> lock(mutex);
	fleetTerritoryDistances[&territory] = fleetDistances;
}


std::unordered_map<const Territory*, std::unordered_map<int, std::vector<NavalFleet*>>> PlayerAIContext::getFleetBorderDistances(int maxDist)
{
	//WARNING! RETURNS DISTANCES TO LAND TERRITORIES AS WELL!!! SHOULD PROBALY EXCLUDE I WOULD IMAGINE!!!!!!!!!!
	
	std::mutex mutex;  // Declare a mutex for thread safety.
	std::vector<std::thread> threads;  // Store threads for joining later

	// minBuckets is chosen based upon observation of maximum buckets needed when running game.
	// In future may need to alter or make more dynamic
	const size_t minBuckets = 50;
	std::unordered_map<const Territory*, std::unordered_map<int, std::vector<NavalFleet*>>> fleetBorderDistances(minBuckets);
	const std::vector<Territory*> borderTerritories = getBorderTerritories();
	// Launch a thread for each territory to determine its distance to each army.
	for(const Territory *territory : borderTerritories)
	{
		threads.emplace_back(&PlayerAIContext::determineTerritoryFleetDistances, this, std::ref(*territory), std::ref(fleetBorderDistances), maxDist, std::ref(mutex));
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
			// For avoiding integer overflow.
			if(distance == INT_MAX)
			{
				continue;
			}
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
			// For avoiding integer overflow.
			if(distance == INT_MAX)
			{
				continue;
			}
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

