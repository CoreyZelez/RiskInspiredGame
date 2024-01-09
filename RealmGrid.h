#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"

class RealmGrid
{
public:
	RealmGrid();

	void draw(sf::RenderWindow &window) const;

	void updateGrid();

	// Adds parameter grid positions to this grid member.
	void addGrid(const Grid &grid);
	// Removes parameter grid positions from grid member.
	void removeGrid(const Grid &grid);

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;

	void setGridColor(const sf::Color &color);
	void setGridColorDefault();

private:
	const sf::Color defaultGridColor;
	Grid grid;
};

