#include "CompositeGrid.h"

void CompositeGrid::draw(sf::RenderWindow & window)
{
}

void CompositeGrid::addGrid(const Grid &grid)
{
	grids.push_back(grid);
	addAdjacencies(grids.back());
	addSubBorders(grids.back());
}

// Removes grid with the specified id.
void CompositeGrid::removeGrid(int id)
{
	// get grid with id. 
	// this will be the same id as the id of the associated territory grid for instance
	// in the case of realm grids made up of multiple territory grids.
	// could alse be a lower estate grid for instance for estate grid. though this second example not needed so far.

	// removeSubBorders(grid)

	// removeAdjacencies(grid)
}

// Adds the adjacencies to the grid.
void CompositeGrid::addAdjacencies(const Grid & grid)
{
}

// Removes the adjacencies with the grid.
void CompositeGrid::removeAdjacencies(const Grid &grid)
{
}

// Removes sub-borders associated with the grid.
void CompositeGrid::removeSubBorders(const Grid &grid)
{
}
