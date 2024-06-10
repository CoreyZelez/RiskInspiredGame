//#pragma once
//#include "Grid.h"
//#include <set>
//#include <map>
//#include <memory>
//#include <list>
//
//enum class BorderMode
//{
//	noBorders,
//	feintBorders,
//	darkBorders
//};
//
//class CompositeGrid
//{
//public:
//	void draw(sf::RenderWindow &window);
//
//	void addGrid(const Grid &grid);
//	void removeGrid(const Grid &grid);
//
//private:
//	std::unordered_set<Grid> grids;
//	std::map<const Grid*, std::unordered_set<const Grid*>> gridAdjacencies;
//	std::unordered_set<sf::Vector2i> borders;
//	std::unordered_set<sf::Vector2i> subBorders;
//
//	void addAdjacencies(const Grid &grid);
//	void removeAdjacencies(const Grid &grid);
//	// Handles border and sub-border changes due to composite grid for added or removed grid.
//	void updateBorders(const Grid &grid, bool added);
//};
