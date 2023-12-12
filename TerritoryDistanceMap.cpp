#include "TerritoryDistanceMap.h"
#include "Territory.h"
#include <assert.h>
#include <queue>
#include <iostream>

TerritoryDistanceMap::TerritoryDistanceMap(Territory& territory)
	: territory(territory)
{
}

void TerritoryDistanceMap::calculateDistances(const std::vector<Territory*>& territories)
{
	distances = calculateDistancesBFS(territories);
}

int TerritoryDistanceMap::getDistance(const Territory &territory) const
{
	// No connecting path exists.
	if(distances.count(&territory) == 0)
	{
		return INT_MAX;
	}

	return distances[&territory];
}

void TerritoryDistanceMap::addAdjacencies(std::vector<Territory*> &territories)
{
	for(Territory* territory : territories)
	{
		if(sharesBorder(*territory))
		{
			adjacencies.insert(territory);
		}
	}
}

bool TerritoryDistanceMap::isAdjacent(const Territory *territory) const
{
	return adjacencies.count(const_cast<Territory*>(territory)) == 1;
}

const std::set<Territory*>& TerritoryDistanceMap::getAdjacencies() const
{
	return adjacencies;
}

std::set<Territory*>& TerritoryDistanceMap::getAdjacencies()
{
	return adjacencies;
}

std::map<const Territory*, int> TerritoryDistanceMap::calculateDistancesBFS(const std::vector<Territory*>& territories)
{
	// Restrict maxDepth as distance of greater value is not useful for us.
	const int maxDepth = 12;

	std::queue<std::pair<const Territory*, int>> q;
	std::map<const Territory*, bool> visited;
	std::map<const Territory*, int> distances;

	// Initialize distances and visited map.
	for(const Territory* t : territories)
	{
		visited[t] = false;
		distances[t] = -1; // Initialize distances to -1 (unreachable).
	}

	// Starting territory has a distance of 0 from itself.
	distances[&territory] = 0;
	visited[&territory] = true;

	q.push({ &territory, 0 });

	while(!q.empty())
	{
		const Territory* currentTerritory = q.front().first;
		int currentDepth = q.front().second;
		q.pop();

		// Check if the current depth exceeds the maximum depth.
		if(currentDepth >= maxDepth)
		{
			continue; // Skip exploring further at this depth.
		}

		// Iterate through adjacent territories.
		for(const Territory* adjacentTerritory : currentTerritory->getDistanceMap().getAdjacencies())
		{
			if(!visited[adjacentTerritory])
			{
				distances[adjacentTerritory] = distances[currentTerritory] + 1;
				visited[adjacentTerritory] = true;
				q.push({ adjacentTerritory, currentDepth + 1 }); // Enqueue adjacent territory with incremented depth.
			}
		}
	}

	return distances;
}

bool TerritoryDistanceMap::sharesBorder(const Territory &territory) const
{
	return this->territory.getGrid().sharesBorder(territory.getGrid());
}