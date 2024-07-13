#pragma once
#include "CompositeGrid.h"

enum class GridType
{
	land,
	naval,
	landSiege
};

class RealmGrid
{
public:
	RealmGrid();

	void draw(sf::RenderWindow& window) const;

	void update();

	void setColor(const sf::Color& color);
	void setColorDefault();

	void setGridType(int id, GridType type);

	void addGrid(const Grid& grid, GridType type);
	void addGrid(const CompositeGrid& grid, GridType type);
	void removeGrid(int id);
	void removeGrid(const CompositeGrid& grid);
	void removeGrid(const RealmGrid& grid);
	// Removes grid with given id and adds to target grid.
	void transferGrid(RealmGrid& target, int gridId, GridType type);

	bool containsPosition(const sf::Vector2f position) const;
	bool isOutdated() const;


private:
	CompositeGrid grid;
	std::map<int, GridType> gridTypes;  // Types of each grid in composite grid.
	sf::Color defaultColor;
	sf::Color color;
	const double subBorderDarkeningFactor = 0.72;
	const double navalDarkeningFactor = 0.55;
	const double siegeDarkeningFactor = 0.82;

	sf::Color getColor(GridType type);
};

// Transfers subgrid associated with given id from source to target.
void transferGrid(RealmGrid& source, RealmGrid& target, int gridId);

