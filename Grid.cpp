#include "Grid.h"
#include "Direction.h"
#include <assert.h>

int Grid::currId = 1;

const std::vector<sf::Vector2i> adjacencyOffsets = {
		sf::Vector2i(1,0),
		sf::Vector2i(-1, 0),
		sf::Vector2i(0,1),
		sf::Vector2i(0,-1) };

Grid::Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> &positions, const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions)
	: id(currId++), borderPositions(borderPositions)
{
}

void Grid::update()
{
	updateBorderVertices();
	updateInteriorVertices();
}

// Does not actually change the color of the vertices. This operation is postponed until update call.
void Grid::setBorderColor(const sf::Color &color)
{
	borderColor = color;
}

// Does not actually change the color of the vertices. This operation is postponed until update call.
void Grid::setInteriorColor(const sf::Color &color)
{
	interiorColor = color;
}

const sf::Color & Grid::getInteriorColor() const
{
	return interiorColor;
}

int Grid::getId() const
{
	return id;
}

bool Grid::sameId(const Grid &grid) const
{
	return id == grid.id;
}

const sf::VertexArray & Grid::getVertices() const
{
	return vertices;
}

bool Grid::isAdjacent(const Grid & grid) const
{
	return false;
}

// Updates colors of all border vertices at once. Border vertex colors are not changed when the change border color
// function is called. This function handles this and should be invoked once before drawing of grid. This potentially 
// improves efficiency as a grid color could change multiple times before a draw call when only the final color of the 
// grid before the draw call is relevant.
void Grid::updateBorderVertices()
{
	if(borderVertices.getVertexCount() > 0 && borderVertices[0].color != borderColor)
	{
		for(int i = 0; i < interiorVertices.getVertexCount(); ++i)
		{
			borderVertices[i].color = borderColor;
		}
	}
}

// Updates colors of all interior vertices at once. See updateBorderVertices description for usage.
void Grid::updateInteriorVertices()
{
	if(interiorVertices.getVertexCount() > 0 && interiorVertices[0].color != interiorColor)
	{
		for(int i = 0; i < interiorVertices.getVertexCount(); ++i)
		{
			interiorVertices[i].color = interiorColor;
		}
	}
}

// Returns true if position is not a border position and is adjacent to a border position.
bool isBorderAdjacent(const sf::Vector2i &position,
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions, bool includeBorders)
{
	if(!includeBorders && borderPositions.count(position) == 1)
	{
		return false;
	}
	else if(borderPositions.count(sf::Vector2i(position.x - 1, position.y)) == 1)
	{
		return true;
	}
	else if(borderPositions.count(sf::Vector2i(position.x + 1, position.y)) == 1)
	{
		return true;
	}
	else if(borderPositions.count(sf::Vector2i(position.x, position.y - 1)) == 1)
	{
		return true;
	}
	else if(borderPositions.count(sf::Vector2i(position.x, position.y + 1)) == 1)
	{
		return true;
	}

	return false;
}

bool isIsolated(const sf::Vector2i &position, const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions)
{
	// Only non-border positions are considered isolated.
	if(borderPositions.count(position) == 1)
	{
		return false;
	}

	// Check if non-border position is surrounded by border positions.
	bool singular = true;
	for(const sf::Vector2i &borderOffset : adjacencyOffsets)
	{
		if(borderPositions.count(borderOffset) == 0)
		{
			return false;
		}
	}

	return true;
}

Direction initialClockwiseDirection(const sf::Vector2i &offsetFromBorder)
{
	// Ensure border is on left side of direction of travel.
	if(offsetFromBorder.x == -1)
	{
		return Direction::down;
	}
	else if(offsetFromBorder.x == 1)
	{
		return Direction::up;
	}
	else if(offsetFromBorder.y == -1)
	{
		return Direction::left;
	}
	else
	{
		return Direction::right;
	}
}

void extractInteriorPolygon(const sf::Vector2i &start, const sf::Vector2i &offsetFromBorder, std::vector<std::vector<sf::Vector2i>>& polygons,
	const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions, std::unordered_set<sf::Vector2i, Vector2iHash>& traversedInteriorPositions)
{
	Direction direction = initialClockwiseDirection(offsetFromBorder);  // Direction of next point to traverse.
	std::vector<sf::Vector2i> polygon;
	sf::Vector2i curr = start;

	// Traverse clockwise to determine interior polygon.
	while(true)
	{
		sf::Vector2i next = nextPosition(curr, direction);

		// Direction has to be changed if next position in previous direction is invalid.
		bool changeDirection = false;
		if(!isBorderAdjacent(next, borderPositions))
		{
			changeDirection = true;
		}

		// Determine if next position in direction is border adjacent. Otherwise change direction.
		if(changeDirection)
		{
			// Initially set direction such that it is a 90 degree left turn from the previous direction.
			--direction;

			// Check all four direction for next valid move.
			// Priority is given to 90 degree left turn, then 90 degree right turn, then 180 degree turn.
			for(int i = 1; i <= 4; ++i)
			{
				// Skip direction already checked.
				if(i == 2)
				{
					++direction;
					continue;
				}

				next = nextPosition(curr, direction);

				// A valid direction must be found.
				assert(i < 4 || isBorderAdjacent(next, borderPositions));

				if(!isBorderAdjacent(next, borderPositions))
				{
					++direction;
				}
				else
				{
					break;
				}
			}
		}

		traversedInteriorPositions.insert(curr);

		if(changeDirection)
		{
			polygon.push_back(curr);
		}

		curr = next;

		// Terminate traversal when start is reached again and no other directions from start are untraversed.
		// If an adjacent position from start is untraversed, the direction of traversal will be set towards
		// this adjacent position with priority for clockwise traversal.
		if(curr == start)
		{
			Direction tempDirection = direction;
			bool terminateTraversal = true;
			// Check each direction at start position for possible traversal.
			for(int i = 1; i <= 4; ++i)
			{
				++tempDirection;
				const sf::Vector2i position = nextPosition(curr, tempDirection);
				if(traversedInteriorPositions.count(position) == 0 && isBorderAdjacent(position, borderPositions))
				{
					// Algorithm will traverse in this untraversed direction from the start position.
					terminateTraversal = false;
					if(tempDirection != direction)
					{
						direction = tempDirection;
						// Only add position to polygon if direction changed.
						polygon.push_back(start);
					}
					break;
				}
			}

			if(terminateTraversal)
			{
				polygons.push_back(polygon);
				return;
			}
		}
	}
}

std::vector<std::vector<sf::Vector2i>> extractInteriorPolygons(const std::unordered_set<sf::Vector2i, Vector2iHash> &positions, 
	const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions)
{
	std::vector<std::vector<sf::Vector2i>> polygons;  // Vertices of polygons arranged clockwise.
	std::unordered_set<sf::Vector2i, Vector2iHash> traversedInteriorPositions = {};  // Traversed adjacent points to border points in interior of polygon.

	// Iterate through each border position and check for adjacent point inside interior of any polygon specified by borders.
	// Upon finding such an interior point, traverse interior along border clockwise, forming vector of vertices of interior polygon.
	for(const sf::Vector2i &border : borderPositions)
	{
		for(const sf::Vector2i& offsetFromBorder : adjacencyOffsets)
		{
			// Start is an adjacent point to the border position.
			const sf::Vector2i start = border + offsetFromBorder;

			if(borderPositions.count(start) == 1)
			{
				continue;
			}

			if(traversedInteriorPositions.count(start) == 1 || positions.count(start) == 0)
			{
				continue;
			}
			else if(isIsolated(start, borderPositions))
			{
				// Add polygon with just start position as it is surrounded by borders.
				traversedInteriorPositions.insert(start);
				polygons.push_back({ start });
				continue;
			}

			extractInteriorPolygon(start, offsetFromBorder, polygons, borderPositions, traversedInteriorPositions);

			// Break at end as other adjacencies to border do not lie in an interior, or lie in interior just traversed.
			break;
		}
	}

	return polygons;
}
