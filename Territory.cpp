#include "Territory.h"
#include "LandedEstate.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

Territory::Territory(int id, Grid grid)
	: Territory(id, grid, nullptr)
{
}

Territory::Territory(int id, sf::Color color)
	: Territory(id, color, nullptr)
{
}

Territory::Territory(int id, Grid grid, std::unique_ptr<IOccupiable> occupancyHandler)
	: id(id), grid(grid), occupancyHandler(std::move(occupancyHandler))
{
}

Territory::Territory(int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler)
	: id(id), grid(color), occupancyHandler(std::move(occupancyHandler))
{
}

void Territory::assignLandedEstate(const LandedEstate *estate)
{
	assert(this->landedEstate == nullptr);
	landedEstate = estate;
}

void Territory::saveToFile(std::ofstream &file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	grid.saveToFile(file);
	file << "# id" << std::endl;
	file << id << std::endl;
	file << std::endl;
}

void Territory::draw(sf::RenderWindow &window) const
{
	grid.draw(window);
}

IOccupiable* Territory::getOccupancyHandler()
{
	return occupancyHandler.get();
}

const Player* Territory::getEstateOwner() const
{
	if(landedEstate == nullptr)
	{
		return nullptr;
	}
	return landedEstate->getRuler();
}

Grid& Territory::getGrid()
{
	return grid;
}

const Grid& Territory::getGrid() const
{
	return grid;
}

int Territory::getID() const
{
	return id;
}

bool Territory::sharesBorder(const Territory &territory) const
{
	return grid.sharesBorder(territory.grid);
}

void Territory::calculateDistances(const std::vector<Territory*>& territories)
{
	distances = calculateDistancesBFS(territories, false);
}

int Territory::getDistance(const Territory &territory, bool sameType) const
{
	// This function should only be called by subclass when this condition holds.
	assert(sameType == false);

	// No connecting path exists.
	if(distances.count(&territory) == 0)
	{
		return -1;
	}

	return distances[&territory];
}

void Territory::addAdjacencies(std::vector<Territory*> &territories)
{
	for(Territory* territory : territories)
	{
		if(sharesBorder(*territory))
		{
			adjacencies.insert(territory);
		}
	}
}

bool Territory::isAdjacent(const Territory *territory) const
{
	return adjacencies.count(const_cast<Territory*>(territory)) == 1;
}

const std::set<Territory*>& Territory::getAdjacencies(bool sameType) const
{
	// This function should only be called by subclass when this condition holds.
	assert(sameType == false);

	return const_cast<std::set<Territory*>&>(adjacencies);
}

std::set<Territory*>& Territory::getAdjacencies(bool sameType)
{
	// This function should only be called by subclass when this condition holds.
	assert(sameType == false);
	return adjacencies;
}

std::map<const Territory*, int> Territory::calculateDistancesBFS(const std::vector<Territory*>& territories, bool sameType)
{
	std::queue<Territory*> q;
	std::map<Territory*, bool> visited;
	std::map<const Territory*, int> distances;

	// Initialize distances and visited map.
	for(Territory* territory : territories)
	{
		visited[territory] = false;
		distances[territory] = -1; // Initialize distances to -1 (unreachable).
	}

	// Starting territory has a distance of 0 from itself.
	distances[this] = 0;
	visited[this] = true;

	q.push(this);

	while(!q.empty())
	{
		Territory* currentTerritory = q.front();
		q.pop();

		// Iterate through adjacent territories.
		for(const Territory* adjacentTerritory : currentTerritory->getAdjacencies(sameType))
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

int loadTerritoryID(std::ifstream & file)
{
	std::string line;
	int id;

	std::getline(file, line);
	assert(line.compare("# id") == 0);
	file >> id;
	std::getline(file, line);
	return id;
}