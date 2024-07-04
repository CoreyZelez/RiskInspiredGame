#pragma once
#include "Grid.h"
#include <SFML/Graphics.hpp>
#include <set>
#include <map>
#include <memory>
#include <list>
#include <unordered_map>

class CompositeGrid
{
public:
	void draw(sf::RenderWindow &window);

	// Updates grids and reconstructs vertex array.
	void update();

	// Set interior color of grid with specific id.
	void setColor(int gridId, const sf::Color &color);

	void setBorderColor(const sf::Color &color);
	void setSubBorderColor(const sf::Color &color);

	// Adds a grid and specify its interior color.
	void addGrid(const Grid &grid, const sf::Color &color);
	void removeGrid(int gridId);

private:
	std::unordered_map<int, Grid> grids;
	std::unordered_map<int, std::unordered_set<int>> gridAdjacencies;
	std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;
	sf::VertexArray vertices;
	sf::Color subBorderColor;
	sf::Color borderColor;

	bool outdated = false;  // Indicates whether vertices need to be recalculated.

	void addAdjacencies(int id);
	void removeAdjacencies(int id);

	// Check if position apart of some member grid is a border position of the entire composite grid.
	bool isBorderPosition(int gridId, const sf::Vector2i& position);
	// Checks borders of adjacent grids to grid with specified id, for a position.
	bool adjacentGridsContainBorderPosition(int gridId, const sf::Vector2i& position);

	void updateBordersForAddedGrid(int id);
	void updateBordersForRemovedGrid(int id);
};
