#include "MilitaryForce.h"
#include "EditorGrid.h"
#include "Player.h"
#include "Territory.h"
#include "TextureManager.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <queue>
#include <unordered_map>

MilitaryForce::MilitaryForce(Player &player, Territory *territory, unsigned int strength, const std::string &shape)
	: player(player), territory(territory), staminaStrength({ 0, 0, strength }), graphics(shape, *this)
{
	assert(territory != nullptr);
	assert(strength > 0);
	assert(!player.hasLiege());
}

MilitaryForce::MilitaryForce(Player &player, Territory *territory, StaminaArray staminaStrength, const std::string &shape)
	: player(player), territory(territory), staminaStrength(staminaStrength), graphics(shape, *this)
{
	assert(territory != nullptr);
	assert(getTotalStrength() > 0);
}

void MilitaryForce::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

void MilitaryForce::resetStamina()
{
	assert(staminaStrength.size() == 3);
	assert(!isDead());
	assert(getTotalStrength() > 0);
	staminaStrength = { 0, 0, getTotalStrength() };
}

void MilitaryForce::reduceStrength(unsigned int amount)
{
	for(int stamina = 0; stamina < staminaStrength.size(); ++stamina)
	{
		if(amount > staminaStrength[stamina])
		{
			amount -= staminaStrength[stamina];
			staminaStrength[stamina] = 0;
		}
		else
		{
			staminaStrength[stamina] -= amount;
			amount = 0;
			break;
		}
	}

	checkDeath();

	// Update graphics sprite of military.
	graphics.update();
}

void MilitaryForce::clearStrength()
{
	for(int stamina = 0; stamina < staminaStrength.size(); ++stamina)
	{
		staminaStrength[stamina] = 0;
	}

	notifyObservers(vacatedMilitary);
}

std::array<unsigned int, 3> MilitaryForce::getStaminaStrength() const
{
	return staminaStrength;
}

StaminaArray MilitaryForce::expendStrength(unsigned int amount, const Territory &territory)
{
	const std::pair<int, int> minMaxStaminaCost = calculateMinMaxStaminaCost(territory);
	const int minStaminaCost = minMaxStaminaCost.first;
	const int maxStaminaCost = minMaxStaminaCost.second;
	StaminaArray newStaminaStrength = { 0 };
	for(int stamina = minStaminaCost; stamina < staminaStrength.size(); ++stamina)
	{
		const int newStamina = std::max(stamina - maxStaminaCost, 0);
		if(staminaStrength[stamina] < amount)
		{
			amount -= staminaStrength[stamina];
			newStaminaStrength[newStamina] = staminaStrength[stamina];
			staminaStrength[stamina] = 0;
		}
		else
		{
			newStaminaStrength[newStamina] = amount;
			staminaStrength[stamina] -= amount;
			amount = 0;
			break;
		}
	}
	graphics.update();
	return newStaminaStrength;
}

void MilitaryForce::increaseStrength(unsigned int strengthAmount)
{
	staminaStrength[staminaStrength.size() - 1] += strengthAmount;
	graphics.update();
}

void MilitaryForce::increaseStrength(std::array<unsigned int, 3> staminaStrength)
{
	for(int stamina = 0; stamina < this->staminaStrength.size(); ++stamina)
	{
		this->staminaStrength[stamina] += staminaStrength[stamina];
	}
	graphics.update();
}

unsigned int MilitaryForce::getStrength(int minStamina) const
{
	unsigned int strengthSum = 0;
	for(int stamina = minStamina; stamina < staminaStrength.size(); ++stamina)
	{
		strengthSum += staminaStrength[stamina];
	}
	return strengthSum;
}

Player& MilitaryForce::getOwner() const
{
	return player;
}

unsigned int MilitaryForce::getTotalStrength() const
{
	return getStrength(0);
}

bool MilitaryForce::isDead() const
{
	return getTotalStrength() == 0;
}

void MilitaryForce::checkDeath()
{
	if(isDead())
	{
		notifyObservers(vacatedMilitary);
	}
}

Territory& MilitaryForce::getTerritory() 
{
	return *territory;
}

const Territory& MilitaryForce::getTerritory() const
{
	return *territory;
}

void MilitaryForce::setTerritory(Territory* territory)
{
	this->territory = territory;
}

void MilitaryForce::updateDiplomacyForAttack(Player &enemyPlayer)
{
	if(!sameUpperRealm(&enemyPlayer, &player))
	{
		if(&enemyPlayer == nullptr)
		{
			return;
		}
		// Only handle diplomacy for upper most liege.
		// In future may want to handle diplomacy for all lieges in hierarchy of given location estate ownership.
		Player &upperLiege = enemyPlayer.getUpperLiege();
		upperLiege.addAttackHistory(player);
		player.addAttackHistory(upperLiege);
	}
}

int MilitaryForce::getMaximumStamina() const
{
	return staminaStrength.size();
}

bool MilitaryForce::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

void MilitaryForce::setSpritePosition(sf::Vector2f position)
{
	graphics.setPosition(position);
}

Territory* nearestAdjacentControlledTerritoryDijkstra(Territory& sourceTerritory, Territory& targetTerritory, int maxDist)
{
	// Ensure territories have the same owner
	assert(sourceTerritory.getUpperController() == targetTerritory.getUpperController());

	// Consider upper realm ruler.
	const Player* player = sourceTerritory.getUpperController();

	// Create a priority queue (min-heap) to select territories with the smallest tentative distance
	std::priority_queue<std::pair<int, Territory*>, std::vector<std::pair<int, Territory*>>, std::greater<>> pq;

	// Create a map to store tentative distances
	std::unordered_map<Territory*, int> distanceMap;

	// Create a map to store territories along the path
	std::unordered_map<Territory*, Territory*> pathMap;

	// Initialize distances
	pq.push({ 0, &sourceTerritory });
	distanceMap[&sourceTerritory] = 0;

	while(!pq.empty())
	{
		// Get the territory with the smallest tentative distance
		Territory* currentTerritory = pq.top().second;
		int currentDistance = pq.top().first;
		pq.pop();

		// Check if the current territory is the targetTerritory
		if(currentTerritory == &targetTerritory)
		{
			// Traverse back along the path to find the adjacent territory to the source
			Territory* adjacentToSource = nullptr;
			while(currentTerritory != nullptr)
			{
				if(currentTerritory->getDistanceMap().isAdjacent(&sourceTerritory))
				{
					adjacentToSource = currentTerritory;
					break;
				}
				currentTerritory = pathMap[currentTerritory];
			}

			return adjacentToSource;
		}

		// Explore adjacencies.
		for(Territory* adjacency : currentTerritory->getDistanceMap().getAdjacencies())
		{
			// Check if the neighbor is friendly.
			if(adjacency->getUpperController() == player)
			{
				// Calculate the tentative distance to the adjacency.
				int tentativeDistance = currentDistance + 1;

				// Update the tentative distance if it's shorter.
				if(distanceMap.find(adjacency) == distanceMap.end() || tentativeDistance < distanceMap[adjacency])
				{
					distanceMap[adjacency] = tentativeDistance;
					pq.push({ tentativeDistance, adjacency });
					pathMap[adjacency] = currentTerritory;
				}
			}
		}

		// Check if the maximum distance is exceeded.
		if(currentDistance >= maxDist)
		{
			return nullptr;
		}
	}

	// No path exists.
	return nullptr;
}

Territory* nearestAdjacentControlledTerritoryDijkstra(Territory &sourceTerritory, Territory &targetTerritory, int maxDist, TerritoryType territoryType)
{
	// Ensure territories have the same owner.
	assert(sourceTerritory.getUpperController() == targetTerritory.getUpperController());
	// Ensure territories of correct type.
	assert(sourceTerritory.getType() == territoryType && targetTerritory.getType() == territoryType);

	// Consider upper realm ruler.
	const Player* player = sourceTerritory.getUpperController();

	// Create a priority queue (min-heap) to select territories with the smallest tentative distance.
	std::priority_queue<std::pair<int, Territory*>, std::vector<std::pair<int, Territory*>>, std::greater<>> pq;

	// Create a map to store tentative distances.
	std::unordered_map<Territory*, int> distanceMap;

	// Create a map to store territories along the path.
	std::unordered_map<Territory*, Territory*> pathMap;

	// Initialize distances.
	pq.push({ 0, &sourceTerritory });
	distanceMap[&sourceTerritory] = 0;

	while(!pq.empty())
	{
		// Get the territory with the smallest tentative distance.
		Territory* currentTerritory = pq.top().second;
		assert(currentTerritory->getType() == territoryType);
		int currentDistance = pq.top().first;
		pq.pop();

		// Check if the current territory is the targetTerritory.
		if(currentTerritory == &targetTerritory)
		{
			// Traverse back along the path to find the adjacent territory to the source.
			Territory* adjacentToSource = nullptr;
			while(currentTerritory != nullptr)
			{
				if(currentTerritory->getDistanceMap().isAdjacent(&sourceTerritory))
				{
					adjacentToSource = currentTerritory;
					break;
				}
				currentTerritory = pathMap[currentTerritory];
			}

			return adjacentToSource;
		}

		// Explore neighbors.
		for(Territory* adjacency : currentTerritory->getDistanceMap().getAdjacencies())
		{
			if(adjacency->getType() != territoryType)
			{
				continue;
			}
			// Check if the adjacency has the same owner as sourceTerritory and targetTerritory.
			if(adjacency->getUpperController() == player)
			{
				// Calculate the tentative distance to the adjacency.
				int tentativeDistance = currentDistance + 1;

				// Update the tentative distance if it's shorter.
				if(distanceMap.find(adjacency) == distanceMap.end() || tentativeDistance < distanceMap[adjacency])
				{
					distanceMap[adjacency] = tentativeDistance;
					pq.push({ tentativeDistance, adjacency });
					pathMap[adjacency] = currentTerritory;
				}
			}
		}

		// Check if the maximum distance is exceeded.
		if(currentDistance >= maxDist)
		{
			return nullptr;
		}
	}

	// No path exists.
	return nullptr;
}
