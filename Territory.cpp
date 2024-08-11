#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "Player.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

Territory::Territory(TerritoryType type, int id, Grid grid)
	: Territory(type, id, grid, nullptr)
{
}

Territory::Territory(TerritoryType type, int id, Grid grid, std::unique_ptr<ITerritoryOccupancy> occupancyHandler)
	: type(type), id(id), grid(grid), occupancyHandler(std::move(occupancyHandler)), distanceMap(*this)
{
}

void Territory::calculateDistances(const std::vector<Territory*>& territories)
{
	distanceMap.calculateDistances(territories);
}

void Territory::setDrawMode(TerritoryDrawMode mode)
{
	drawMode = mode;
}

TerritoryDrawMode Territory::getDrawMode() const
{
	return drawMode;
}

void Territory::draw(sf::RenderWindow &window) const
{
	grid.draw(window);
}

ITerritoryOccupancy* Territory::getOccupancyHandler()
{
	return occupancyHandler.get();
}

const ITerritoryOccupancy* Territory::getOccupancyHandler() const
{
	return occupancyHandler.get();
}

TerritoryDistanceMap& Territory::getDistanceMap()
{
	return distanceMap;
}

const TerritoryDistanceMap & Territory::getDistanceMap() const
{
	return distanceMap;
}

Player* Territory::getUpperController()
{
	if(getController() != nullptr)
	{
		return &getController()->getUpperLiege();
	}

	return nullptr;
}

const Player* Territory::getUpperController() const
{
	if(getController() != nullptr)
	{
		return &getController()->getUpperLiege();
	}

	return nullptr;
}

Player* Territory::getController()
{
	return occupancyHandler.get()->getController();
}

const Player* Territory::getController() const
{
	return occupancyHandler.get()->getController();
}

Grid& Territory::getGrid()
{
	return grid;
}

const Grid& Territory::getGrid() const
{
	return grid;
}

int Territory::getId() const
{
	return id;
}

TerritoryType Territory::getType() const
{
	return type;
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
