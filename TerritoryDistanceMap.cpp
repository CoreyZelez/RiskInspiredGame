#include "TerritoryDistanceMap.h"
#include "Territory.h"
#include <assert.h>
#include <queue>

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
		return -1;
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
	std::queue<const Territory*> q;
	std::map<const Territory*, bool> visited;
	std::map<const Territory*, int> distances;

	// Initialize distances and visited map.
	for(const Territory* territory : territories)
	{
		visited[territory] = false;
		distances[territory] = -1; // Initialize distances to -1 (unreachable).
	}

	// Starting territory has a distance of 0 from itself.
	distances[&territory] = 0;
	visited[&territory] = true;

	q.push(&territory);

	while(!q.empty())
	{
		const Territory* currentTerritory = q.front();
		q.pop();

		// Iterate through adjacent territories.
		for(const Territory* adjacentTerritory : currentTerritory->getDistanceMap().getAdjacencies()) 
		{
			if(!visited[const_cast<Territory*>(adjacentTerritory)])
			{
				distances[const_cast<Territory*>(adjacentTerritory)] = distances[currentTerritory] + 1;
				visited[const_cast<Territory*>(adjacentTerritory)] = true;
				q.push(const_cast<Territory*>(adjacentTerritory)); // Enqueue adjacent territory.
			}
		}
	}

	return distances;
}

bool TerritoryDistanceMap::sharesBorder(const Territory &territory) const
{
	return this->territory.getGrid().sharesBorder(territory.getGrid());
}