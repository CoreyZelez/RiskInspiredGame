#pragma once
#include <SFML/Graphics.hpp>
#include "EditorGrid.h"

class RealmGrid
{
public:
	RealmGrid();

	void draw(sf::RenderWindow &window) const;

	void updateGrid();

	// Adds parameter grid positions to this grid member.
	void addGrid(const EditorGrid &grid);
	// Removes grid positions from this grid.
	void removeGrid(const EditorGrid &grid);
	// Removes realm grid from this realm grid.
	void removeGrid(const RealmGrid &realmGrid);

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;

	bool isOutdated() const;

	void setGridColor(const sf::Color &color);
	void setGridColorDefault();

private:
	const sf::Color defaultGridColor;
	EditorGrid grid;
};

