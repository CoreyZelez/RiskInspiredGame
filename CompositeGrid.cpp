#include "CompositeGrid.h"
#include "Grid.h"
#include "UtilitySFML.h"
#include <assert.h>

const std::vector<sf::Vector2i> adjacencyOffsets = {
		sf::Vector2i(1,0),
		sf::Vector2i(-1, 0),
		sf::Vector2i(0,1),
		sf::Vector2i(0,-1),
		sf::Vector2i(1,-1),
		sf::Vector2i(1, 1),
		sf::Vector2i(-1,-1),
		sf::Vector2i(-1,1)
};

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

	vertices.clear();
	vertices.setPrimitiveType(sf::Triangles);

	// Batch vertices of each grid.
	for(auto &[id, grid] : grids)
	{
		appendVertexArray(vertices, grid.getVertices());
	}

	// Batch border vertices.
	for (const sf::Vector2i& position : borderPositions)
	{
		const float left = position.x * GRID_SQUARE_SIZE;
		const float right = (position.x * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;
		const float top = (position.y * GRID_SQUARE_SIZE);
		const float bottom = (position.y * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;

		vertices.append(sf::Vertex(sf::Vector2f(left, top), borderColor));
		vertices.append(sf::Vertex(sf::Vector2f(left, bottom), borderColor));
		vertices.append(sf::Vertex(sf::Vector2f(right, top), borderColor));
		vertices.append(sf::Vertex(sf::Vector2f(right, top), borderColor));
		vertices.append(sf::Vertex(sf::Vector2f(right, bottom), borderColor));
		vertices.append(sf::Vertex(sf::Vector2f(left, bottom), borderColor));
	}

	outdated = false;
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
	updateBordersForAddedGrid(id);

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
	updateBordersForRemovedGrid(id);
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

bool CompositeGrid::isBorderPosition(int gridId, const sf::Vector2i& position)
{
	const Grid& grid = grids[gridId];
	auto& adjacentGridIds = gridAdjacencies[gridId];

	assert(grid.containsBorderPosition(position));

	const std::vector<sf::Vector2i> adjacentPositions = {
		sf::Vector2i(position.x + 1, position.y),
		sf::Vector2i(position.x + 1, position.y),
		sf::Vector2i(position.x + 1, position.y + 1),
		sf::Vector2i(position.x + 1, position.y - 1),
		sf::Vector2i(position.x - 1, position.y + 1),
		sf::Vector2i(position.x - 1, position.y - 1),
		sf::Vector2i(position.x, position.y + 1),
		sf::Vector2i(position.x, position.y - 1)
	};

	for (const sf::Vector2i& adjacencyOffset : adjacencyOffsets)
	{
		const sf::Vector2i adjacentPosition = position + adjacencyOffset;

		if (grid.containsPosition(adjacentPosition) || adjacentGridsContainBorderPosition(gridId, adjacentPosition))
		{
			continue;
		}
		else
		{
			// Adjacent position not apart of composite grid so position is a border position.
			return true;
		}
	}

	return false;
}

bool CompositeGrid::adjacentGridsContainBorderPosition(int gridId, const sf::Vector2i& position)
{
	for (int adjacentGridId : gridAdjacencies[gridId])
	{
		const Grid& adjacentGrid = grids[adjacentGridId];

		if (adjacentGrid.containsBorderPosition(position))
		{
			return true;
		}
	}

	return false;
}

void CompositeGrid::updateBordersForAddedGrid(int id)
{
	const Grid& addedGrid = grids[id];

	// Check each border position of adjacent grids to determine if still border position.
	for (int adjacentGridId : gridAdjacencies[id])
	{
		const Grid& adjacentGrid = grids[adjacentGridId];

		for (const sf::Vector2i& position : adjacentGrid.getBorderPositions())
		{
			if (borderPositions.count(position) == 0)
			{
				continue;
			}
			else if(addedGrid.positionAdjacentToBorder(position) && !isBorderPosition(adjacentGridId, position))
			{
				// Position is no longer on composite grid border.
				borderPositions.erase(position);
			}
		}
	}

	// Determine positions of added grid which are border positions of composite grid.
	for (const sf::Vector2i& position : addedGrid.getBorderPositions())
	{
		if (isBorderPosition(id, position))
		{
			borderPositions.insert(position);
		}
	}

	outdated = true;
}

void CompositeGrid::updateBordersForRemovedGrid(int id)
{
	const Grid& removedGrid = grids[id];

	// Add border positions from adjacent grids to removed grid.
	for (int adjacentGridId : gridAdjacencies[id])
	{
		const Grid& adjacentGrid = grids[adjacentGridId];

		// Any border position in adjacent grid that is adjacent to removed grid becomes part of composite grid border.
		for (const sf::Vector2i& borderPosition : adjacentGrid.getBorderPositions())
		{
			if (removedGrid.positionAdjacentToBorder(borderPosition))
			{
				borderPositions.insert(borderPosition);
			}
		}
	}

	// Erase border positions from removed grid.
	for (const sf::Vector2i& borderPosition : removedGrid.getBorderPositions())
	{
		borderPositions.erase(borderPosition);
	}
	
	outdated = true;
}