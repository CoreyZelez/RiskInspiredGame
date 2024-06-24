#include "CompositeGrid.h"
#include "Grid.h"
#include "UtilitySFML.h"
#include <assert.h>

void CompositeGrid::draw(sf::RenderWindow &window)
{
	window.draw(vertices);
}

void CompositeGrid::update()
{
	if(!outdated)
	{
		return;
	}

	for(auto &[id, grid] : grids)
	{
		grid.update();
	}

	outdated = false;

	vertices.clear();
	vertices.setPrimitiveType(sf::Triangles);

	// Batch vertices of each grid.
	for(auto &[id, grid] : grids)
	{
		appendVertexArray(vertices, grid.getVertices());
	}

	/***********************************************************************************************
	POTENTIALLY GOOD OPTIMISATION IDEA.
	FOR EACH GRID TRACK ITS START POINT IN THE VERTEX ARRAY.
	THEN WHEN A GRID COLOR BECOMES OUTDATED, CAN SIMPLY ITERATE THROUGH GRIDS SPECIFIC VERTICES
	AND CHANGE THE COLORS OF THOSE. AVOIDS HAVING TO REBUILD ENTIRE ARRAY.
	************************************************************************************************/
}

void CompositeGrid::setColor(int gridId, const sf::Color &color)
{
	if(grids.at(gridId).getInteriorColor() != color)
	{
		outdated = true;
		grids.at(gridId).setInteriorColor(color);
	}
}

void CompositeGrid::setBorderColor(const sf::Color & color)
{
	if(borderColor != color)
	{
		outdated = true;
		borderColor = color;
	}
}

// Sets the border color of each grid to the desired sub-border color as the grid borders will make up
// the sub-borders of the composite grid.
void CompositeGrid::setSubBorderColor(const sf::Color & color)
{
	if(subBorderColor == color)
	{
		return;
	}

	outdated = true;
	borderColor = color;
	for(auto &[id, grid] : grids)
	{
		grid.setBorderColor(color);
	}
}

void CompositeGrid::addGrid(const Grid &grid, const sf::Color &color)
{
	const int id = grid.getId();

	assert(grids.count(id) == 0);

	grids.insert(std::make_pair(id, grid));

	grids[id].setInteriorColor(color);
	grids[id].setBorderColor(subBorderColor);

	addAdjacencies(id);
	updateBorders(id, true);

	outdated = true;
}

// Removes a grid in grids that shares the same id as the paramter grid (i.e. are identically shaped).
void CompositeGrid::removeGrid(int id)
{
	if(grids.count(id) == 0)
	{
		return;
	}

	outdated = true;

	// Update borders based on adjacencies
	updateBorders(id, false);
	removeAdjacencies(id);
	grids.erase(id);
}

// Adds the adjacencies to the grid with the specified id.
void CompositeGrid::addAdjacencies(int id)
{
	for(const auto &[gridId, grid] : grids)
	{
		if(gridId == id)
		{
			continue;
		}
		else if(grids[id].isAdjacent(grid))
		{
			gridAdjacencies[gridId].insert(id);
			gridAdjacencies[id].insert(gridId);
		}
	}
}

// Removes the adjacencies to the grid with the specified id.
void CompositeGrid::removeAdjacencies(int id)
{
	for(int gridId : gridAdjacencies[id])
	{
		gridAdjacencies[gridId].erase(id);
	}

	gridAdjacencies.erase(id);
}

void CompositeGrid::updateBorders(int id, bool added)
{
	if(added)
	{
		// check border positions of adjacent grids to be removed from composite grid border.
		// check border position of newly added grid to be added to composite grid border.
	}
	else
	{
		// check border positions of adjacent grids to be added to composite grid border.
		// check border position of newly added grid to be removed from composite grid border.
	}
}