#pragma once
#include "Grid.h"
#include <set>
#include <map>
#include <memory>
#include <list>

enum class BorderMode
{
	noBorders,
	feintBorders,
	darkBorders
};

class CompositeGrid
{
public:
	void draw(sf::RenderWindow &window);

	void addGrid(const Grid &grid);
	void removeGrid(int id);

private:
	std::list<Grid> grids;
	std::map<const Grid*, std::vector<const Grid*>> gridAdjacencies;

	void addAdjacencies(const Grid &grid);
	void removeAdjacencies(const Grid &grid);
	void removeSubBorders(const Grid &grid);
	void addSubBorders(const Grid &grid);
};
