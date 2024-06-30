#include "Territory.h"
#include "LandedEstate.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

Territory::Territory(TerritoryType type, int id, EditorGrid grid)
	: Territory(type, id, grid, nullptr)
{
}

Territory::Territory(TerritoryType type, int id, sf::Color color)
	: Territory(type, id, color, nullptr)
{
}

Territory::Territory(TerritoryType type, int id, EditorGrid grid, std::unique_ptr<IOccupiable> occupancyHandler)
	: type(type), id(id), grid(grid), occupancyHandler(std::move(occupancyHandler)), distanceMap(*this)
{
	this->grid.setBorderMode(BorderMode::feintBorders);
}

Territory::Territory(TerritoryType type, int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler)
	: type(type), id(id), grid(color), occupancyHandler(std::move(occupancyHandler)), distanceMap(*this)
{
	this->grid.setBorderMode(BorderMode::feintBorders);
}

void Territory::assignLandedEstate(LandedEstate *estate)
{
	assert(this->landedEstate == nullptr);
	landedEstate = estate;
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

void Territory::saveToFile(std::ofstream &file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	grid.saveToFile(file);
	file << "# id" << std::endl;
	file << id << std::endl;
}

void Territory::draw(sf::RenderWindow &window) const
{
	grid.draw(window);
}

IOccupiable* Territory::getOccupancyHandler()
{
	return occupancyHandler.get();
}

const IOccupiable * Territory::getOccupancyHandler() const
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

Player* Territory::getEstateOwner() 
{
	if(landedEstate == nullptr)
	{
		return nullptr;
	}
	return landedEstate->getRuler();
}

const Player * Territory::getEstateOwner() const
{
	if(landedEstate == nullptr)
	{
		return nullptr;
	}
	return landedEstate->getRuler();
}

EditorGrid& Territory::getGrid()
{
	return grid;
}

const EditorGrid& Territory::getGrid() const
{
	return grid;
}

int Territory::getID() const
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
