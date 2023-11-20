#include "MilitaryForce.h"
#include "Grid.h"
#include "Player.h"
#include "Territory.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <queue>
#include <unordered_map>

MilitaryForce::MilitaryForce(Player &owner, Territory *territory, unsigned int strength, const sf::Texture &texture)
	: owner(owner), territory(territory), staminaStrength({ 0, 0, 0, strength }), graphics(texture, *this)
{
	assert(territory != nullptr);
	assert(strength > 0);
}

MilitaryForce::MilitaryForce(Player & owner, Territory * territory, std::array<unsigned int, 4> staminaStrength, const sf::Texture & texture)
	: owner(owner), territory(territory), staminaStrength(staminaStrength), graphics(texture, *this)
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
	assert(staminaStrength.size() == 4);
	assert(!isDead());
	assert(getTotalStrength() > 0);
	staminaStrength = { 0, 0, 0, getTotalStrength() };
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
	notifyObservers(deadMilitary);
}

std::array<unsigned int, 4> MilitaryForce::getStaminaStrength() const
{
	return staminaStrength;
}

std::array<unsigned int, 4> MilitaryForce::expendStrength(unsigned int amount, const Territory &territory)
{
	const std::pair<int, int> minMaxStaminaCost = calculateMinMaxStaminaCost(territory);
	const int minStaminaCost = minMaxStaminaCost.first;
	const int maxStaminaCost = minMaxStaminaCost.second;
	std::array<unsigned int, 4> newStaminaStrength = { 0 };
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
	assert(amount == 0);  // Should not call function with amount greater than total available strength.
	graphics.update();
	return newStaminaStrength;
}

void MilitaryForce::increaseStrength(unsigned int strengthAmount)
{
	staminaStrength[staminaStrength.size() - 1] += strengthAmount;
	graphics.update();
}

void MilitaryForce::increaseStrength(std::array<unsigned int, 4> staminaStrength)
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
	return owner;
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
		notifyObservers(deadMilitary);
	}
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