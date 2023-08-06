#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

Territory::Territory(TerritoryGraphics graphics)
	: graphics(graphics)
{
}

Territory::Territory(sf::Color color)
	: graphics(color)
{
}

void Territory::saveToFile(std::ofstream &file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	graphics.saveToFile(file);
	file << std::endl;
}

void Territory::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

bool Territory::isEmpty() const
{
	return graphics.isEmpty();
}

void Territory::addSquare(sf::Vector2f position)
{
	graphics.addSquare(position);
}

void Territory::removeSquare(sf::Vector2f position)
{
	graphics.removeSquare(position);
}

const Player *Territory::getOccupant() 
{
	return nullptr;
}

bool Territory::sharesBorder(const Territory &territory) const
{
	return graphics.sharesBorder(territory.graphics);
}

bool Territory::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}

sf::Vector2f Territory::getCenter() const
{
	return graphics.getCenter();
}

//TerritoryGraphics createTerritory(std::ifstream &file)
//{
//	return TerritoryGraphics();
//}
