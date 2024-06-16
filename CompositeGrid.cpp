#include "CompositeGrid.h"
#include "Grid.h"
#include <assert.h>

void CompositeGrid::draw(sf::RenderWindow & window)
{
}

void CompositeGrid::update()
{
	if(!outdated)
	{
		return;
	}

	outdated = false;

	vertices.clear();
	vertices.setPrimitiveType(sf::Triangles);

	// Batch vertices of each grid.
	for(auto &[id, grid] : grids)
	{
		const sf::VertexArray &gridVertices = grid.getVertices();
		for(int i = 0; i < gridVertices.getVertexCount(); ++i)
		{
			vertices.append(gridVertices[i]);
		}
	}
}

void CompositeGrid::setColor(int gridId, const sf::Color &color)
{
	if(grids.count(gridId) == 1)
	{
		grids[gridId].setInteriorColor(color);
	}
}

void CompositeGrid::addGrid(const Grid &grid)
{
	const int id = grid.getId();

	assert(grids.count(id) == 0);

	grids.erase(id);
	grids.insert(std::make_pair(id, grid));
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

	}
	else
	{

	}
}