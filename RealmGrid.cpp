#include "RealmGrid.h"
#include "ColorUtility.h"
#include <assert.h>

RealmGrid::RealmGrid()
	: defaultColor(generateRandomRealmColor()), color(defaultColor)
{
}

void RealmGrid::draw(sf::RenderWindow& window) const
{
	grid.draw(window);
}

void RealmGrid::update()
{
	grid.update();
}

void RealmGrid::setColor(const sf::Color& color)
{
	this->color = color;
	grid.setSubBorderColor(color, subBorderDarkeningFactor);

	for(auto& [id, type] : gridTypes)
	{
		grid.setColor(id, getColor(type));
	}
}

void RealmGrid::setColorDefault()
{
	color = defaultColor;
	grid.setSubBorderColor(defaultColor, subBorderDarkeningFactor);

	for(auto& [id, type] : gridTypes)
	{
		grid.setColor(id, getColor(type));
	}
}

void RealmGrid::setGridType(int id, GridType type)
{
	assert(gridTypes.count(id));

	gridTypes[id] = type;
	grid.setColor(id, getColor(type));
}

void RealmGrid::addGrid(const Grid& grid, GridType type)
{
	gridTypes[grid.getId()] = type;
	this->grid.addGrid(grid, getColor(type));
}

void RealmGrid::addGrid(const CompositeGrid& grid, GridType type)
{
	for(int id : grid.getGridIds())
	{
		gridTypes[id] = type;
	}

	this->grid.addGrid(grid, getColor(type));
}

void RealmGrid::removeGrid(int id)
{
	gridTypes.erase(id);
	grid.removeGrid(id);
}

void RealmGrid::removeGrid(const CompositeGrid& grid)
{
	for(int id : grid.getGridIds())
	{
		gridTypes.erase(id);
	}

	this->grid.removeGrid(grid);
}

void RealmGrid::removeGrid(const RealmGrid& realmGrid)
{
	for (int id : realmGrid.grid.getGridIds())
	{
		gridTypes.erase(id);
	}

	grid.removeGrid(realmGrid.grid);
}

void RealmGrid::transferGrid(RealmGrid& target, int gridId, GridType type)
{
	target.addGrid(grid.getGrid(gridId), type);
	removeGrid(gridId);
}

bool RealmGrid::containsPosition(const sf::Vector2f position) const
{
	return grid.containsPosition(position);
}

bool RealmGrid::isOutdated() const
{
	return grid.isOutdated();
}

sf::Color RealmGrid::getColor(GridType type)
{
	switch(type)
	{
	case GridType::land:
		return color;
	case GridType::naval:
		return getDarkenedColor(color, navalDarkeningFactor);
	case GridType::landSiege:
		return getDarkenedColor(color, siegeDarkeningFactor);
	}
}
