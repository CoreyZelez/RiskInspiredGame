#pragma once
#include "Grid.h"
#include <SFML/Graphics.hpp>
#include <set>
#include <map>
#include <memory>
#include <list>
#include <unordered_map>

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
	std::unordered_set<sf::Vector2i, Vector2iHash> borders;
	sf::VertexArray vertices;
	sf::Color subBorderColor;
	sf::Color borderColor;

	bool outdated = false;  // Indicates whether vertices need to be recalculated.

	void addAdjacencies(int id);
	void removeAdjacencies(int id);

	// Handles border and sub-border changes due to added or removed grid with specific id.
	void updateBorders(int id, bool added);
};
