#include "MilitaryForce.h"
#include "Grid.h"
#include "Player.h"
#include "Territory.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <queue>
#include <unordered_map>

MilitaryForce::MilitaryForce(Player &owner, Territory *territory, int strength, const sf::Texture &texture)
	: owner(owner), territory(territory), strength(strength), graphics(texture, this->strength)
{
	assert(territory != nullptr);
	assert(strength > 0);
}

void MilitaryForce::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

void MilitaryForce::adjustStrength(int amount)
{
	/// SHOULD NEVER ADJUST STRENGTH OF DEAD ARMY.
	assert(strength > 0);

	if(strength <= -amount)
	{
		strength = 0;
		notifyObservers(deadMilitary);
	}
	else
	{
		strength += amount;
	}

	graphics.update();
}

int MilitaryForce::getStrength() const
{
	return strength;
}

Player& MilitaryForce::getOwner() const
{
	return owner;
}

bool MilitaryForce::isDead() const
{
	return strength == 0;
}

Territory& MilitaryForce::getTerritory() const
{
	return *territory;
}

void MilitaryForce::setTerritory(Territory* territory)
{
	this->territory = territory;
}

bool MilitaryForce::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

void MilitaryForce::setSpritePosition(sf::Vector2f position)
{
	
	graphics.setPosition(position);
}



Territory* nearestFriendlyAdjacentTerritoryDijkstra(Territory& sourceTerritory, Territory& targetTerritory, int maxDist)
{
	// Ensure territories have the same owner
	assert(sourceTerritory.getEstateOwner() == targetTerritory.getEstateOwner());

	const Player* player = sourceTerritory.getEstateOwner();

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

		// Explore neighbors
		for(Territory* neighbor : currentTerritory->getDistanceMap().getAdjacencies())
		{
			// Check if the neighbor has the same owner as sourceTerritory and targetTerritory
			if(neighbor->getEstateOwner() == player)
			{
				// Calculate the tentative distance to the neighbor
				int tentativeDistance = currentDistance + 1;

				// Update the tentative distance if it's shorter
				if(distanceMap.find(neighbor) == distanceMap.end() || tentativeDistance < distanceMap[neighbor])
				{
					distanceMap[neighbor] = tentativeDistance;
					pq.push({ tentativeDistance, neighbor });
					// Store the path information
					pathMap[neighbor] = currentTerritory;
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