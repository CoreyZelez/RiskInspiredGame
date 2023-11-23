#include "Territory.h"
#include "LandedEstate.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

Territory::Territory(int id, Grid grid, TerritoryType type)
	: Territory(id, grid, nullptr, type)
{
}

Territory::Territory(int id, sf::Color color, TerritoryType type)
	: Territory(id, color, nullptr, type)
{
}

Territory::Territory(int id, Grid grid, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type)
	: id(id), grid(grid), occupancyHandler(std::move(occupancyHandler)), distanceMap(*this), type(type)
{
	this->grid.setBorderMode(BorderMode::feintBorders);
}

Territory::Territory(int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type)
	: id(id), grid(color), occupancyHandler(std::move(occupancyHandler)), distanceMap(*this), type(type)
{
	this->grid.setBorderMode(BorderMode::feintBorders);
}

void Territory::assignLandedEstate(const LandedEstate *estate)
{
	assert(this->landedEstate == nullptr);
	landedEstate = estate;
}

void Territory::calculateDistances(const std::vector<Territory*>& territories)
{
	distanceMap.calculateDistances(territories);
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

TerritoryDistanceMap& Territory::getDistanceMap()
{
	return distanceMap;
}

const TerritoryDistanceMap & Territory::getDistanceMap() const
{
	return distanceMap;
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

TerritoryType Territory::getType() const
{
	return type;
}

int Territory::getID() const
{
	return id;
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