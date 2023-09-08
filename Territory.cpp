#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

Territory::Territory(int id, Grid grid)
	: id(id), grid(grid)
{
}

Territory::Territory(int id, sf::Color color)
	: id(id), grid(color)
{
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

bool Territory::isEmpty() const
{
	return grid.isEmpty();
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

Player *Territory::getOccupant()
{
	return nullptr;
}

bool Territory::sharesBorder(const Territory &territory) const
{
	return grid.sharesBorder(territory.grid);
}

void Territory::calculateDistances(const std::vector<Territory*>& territories)
{
	std::queue<Territory*> q;
	std::map<Territory*, bool> visited;

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
		for(const Territory* adjacentTerritory : currentTerritory->adjacencies) 
		{
			if(!visited[const_cast<Territory*>(adjacentTerritory)]) 
			{
				distances[const_cast<Territory*>(adjacentTerritory)] = distances[currentTerritory] + 1;
				visited[const_cast<Territory*>(adjacentTerritory)] = true;
				q.push(const_cast<Territory*>(adjacentTerritory)); // Enqueue adjacent territory.
			}
		}
	}
}

void Territory::addAdjacencies(const std::vector<Territory*> &territories)
{
	for(const Territory* territory : territories)
	{
		if(sharesBorder(*territory))
		{
			adjacencies.insert(territory);
		}
	}
}

bool Territory::isAdjacent(const Territory *territory) const
{
	return adjacencies.count(territory) == 1;
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}

sf::Vector2f Territory::getCenter() const
{
	return grid.getCenter();
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
