#include "RealmGrid.h"
#include "Utility.h"

RealmGrid::RealmGrid()
	: defaultGridColor(createRandomRealmColor()), grid(defaultGridColor)
{
}

void RealmGrid::draw(sf::RenderWindow & window) const
{
	grid.draw(window);
}

void RealmGrid::updateGrid()
{
	grid.update();
}

void RealmGrid::addGrid(const Grid &grid)
{
	this->grid.addGrid(grid, false);
}

void RealmGrid::removeGrid(const Grid &grid)
{
	this->grid.removeGrid(grid, false);
}

void RealmGrid::removeGrid(const RealmGrid & realmGrid)
{
	removeGrid(realmGrid.grid);
}

bool RealmGrid::containsPosition(const sf::Vector2f & position) const
{
	return grid.containsPosition(position);
}

void RealmGrid::setGridColor(const sf::Color &color)
{
	if(grid.getColor() != color)
	{
		grid.setColor(color);
	}
}

void RealmGrid::setGridColorDefault()
{
	grid.setColor(defaultGridColor);
}
