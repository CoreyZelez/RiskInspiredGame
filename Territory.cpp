#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

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
	drawMilitary(window);
}

bool Territory::isEmpty() const
{
	return grid.isEmpty();
}

Grid& Territory::getGrid()
{
	return grid;
}

const Grid & Territory::getGrid() const
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

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}

sf::Vector2f Territory::getCenter() const
{
	return grid.getCenter();
}

void Territory::drawMilitary(sf::RenderWindow &window) const
{
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
