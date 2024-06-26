#include "Grid.h"
#include "Direction.h"
#include <assert.h>
#include "UtilitySFML.h"

int Grid::currId = 1;

const std::vector<sf::Vector2i> adjacencyOffsets = {
		sf::Vector2i(1,0),
		sf::Vector2i(-1, 0),
		sf::Vector2i(0,1),
		sf::Vector2i(0,-1) };

Grid::Grid(const std::unordered_set<sf::Vector2i, Vector2iHash>& positions)
	: id(currId++), borderPositions(determineBorderPositions(positions))
{
	vertices.setPrimitiveType(sf::Triangles);
	initBorderVertices(borderPositions);
	initInteriorVertices(positions, borderPositions);
	aggregateVertices();
}

void Grid::update()
{
	vertices.setPrimitiveType(sf::Triangles);
	updateBorderVertices();
	updateInteriorVertices();
	aggregateVertices();
}

void Grid::draw(sf::RenderWindow& window) const
{
	window.draw(vertices);
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

void Grid::initBorderVertices(const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions)
{
	borderVertices.setPrimitiveType(sf::Triangles);
	const int numTriangles = borderPositions.size() * 6;
	borderVertices.resize(numTriangles);

	int i = 0;

	for (const sf::Vector2i& position : borderPositions)
	{
		const float left = position.x * GRID_SQUARE_SIZE;
		const float right = (position.x * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;
		const float top = position.y * GRID_SQUARE_SIZE;
		const float bottom = (position.y * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;

		// Pointer to the triangles' vertices of the current tile.
		sf::Vertex* triangles = &borderVertices[i * 6];
		triangles[0].position = sf::Vector2f(left, top);
		triangles[1].position = sf::Vector2f(left, bottom);
		triangles[2].position = sf::Vector2f(right, top);
		triangles[3].position = sf::Vector2f(right, top);
		triangles[4].position = sf::Vector2f(right, bottom);
		triangles[5].position = sf::Vector2f(left, bottom);
		triangles[0].color = borderColor;
		triangles[1].color = borderColor;
		triangles[2].color = borderColor;
		triangles[3].color = borderColor;
		triangles[4].color = borderColor;
		triangles[5].color = borderColor;

		++i;
	}
}

void Grid::initInteriorVertices(const std::unordered_set<sf::Vector2i, Vector2iHash>& positions, const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions)
{
	const std::vector<std::vector<sf::Vector2i>> interiorPolygons = extractInteriorPolygons(positions, borderPositions);
	for (const std::vector<sf::Vector2i>& polygon : interiorPolygons)
	{
		triangulatePolygon(interiorVertices, convertPolygon(polygon), interiorColor);
	}
}

// Updates colors of all border vertices at once. Border vertex colors are not changed when the change border color
// function is called. This function handles this and should be invoked once before drawing of grid. This potentially 
// improves efficiency as a grid color could change multiple times before a draw call when only the final color of the 
// grid before the draw call is relevant.
void Grid::updateBorderVertices()
{
	if(borderVertices.getVertexCount() > 0 && borderVertices[0].color != borderColor)
	{
		for(int i = 0; i < borderVertices.getVertexCount(); ++i)
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

void Grid::aggregateVertices()
{
	vertices.clear();
	appendVertexArray(vertices, borderVertices);
	appendVertexArray(vertices, interiorVertices);
}

bool isBorderAdjacent(const sf::Vector2i &position,
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions, bool includeDiagonals, bool includeBorders)
{
	if (!includeBorders && borderPositions.count(position) == 1)
	{
		return false;
	}

	std::vector<sf::Vector2i> laterals = {
		sf::Vector2i(position.x - 1, position.y),
		sf::Vector2i(position.x + 1, position.y),
		sf::Vector2i(position.x, position.y - 1),
		sf::Vector2i(position.x, position.y + 1)
	};

	std::vector<sf::Vector2i> diagonals = {
		sf::Vector2i(position.x - 1, position.y - 1),
		sf::Vector2i(position.x - 1, position.y + 1),
		sf::Vector2i(position.x + 1, position.y - 1),
		sf::Vector2i(position.x + 1, position.y + 1)
	};

	for (const sf::Vector2i lateralPosition : laterals)
	{
		if (borderPositions.count(lateralPosition ) == 1)
		{
			return true;
		}
	}

	if (includeDiagonals)
	{
		for (const sf::Vector2i diagonalPosition : diagonals)
		{
			if (borderPositions.count(diagonalPosition) == 1)
			{
				return true;
			}
		}
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
		if(borderPositions.count(position + borderOffset) == 0)
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
	Direction currDirection = initialClockwiseDirection(offsetFromBorder);  // Direction of next point to traverse.
	std::vector<sf::Vector2i> polygon;
	sf::Vector2i currPosition = start;

	// Traverse clockwise to determine interior polygon.
	while(true)
	{
		sf::Vector2i nextPosition;
		Direction nextDirection = currDirection;

		// We check for the direction left of the current direction first.
		--nextDirection;

		// Check all four direction for next valid move.
		// Priority is given to 90 degree left turn, then 90 degree right turn, then 180 degree turn.
		for(int i = 1; i <= 4; ++i)
		{
			nextPosition = determineNextPosition(currPosition, nextDirection);

			// A valid direction must be found.
			assert(i < 4 || isBorderAdjacent(nextPosition, borderPositions));

			if(!isBorderAdjacent(nextPosition, borderPositions))
			{
				++nextDirection;
			}
			else
			{
				break;
			}
		}

		traversedInteriorPositions.insert(currPosition);

		if(nextDirection != currDirection)
		{
			polygon.push_back(currPosition);
		}

		currPosition = nextPosition;
		currDirection = nextDirection;

		// Terminate traversal when start is reached again and no other directions from start are untraversed.
		// If an adjacent position from start is untraversed, the direction of traversal will be set towards
		// this adjacent position with priority for clockwise traversal.
		if(currPosition == start)
		{
			bool terminateTraversal = true;

			// Check each direction at start position for possible traversal.
			for(int i = 1; i <= 4; ++i)
			{
				nextPosition = determineNextPosition(currPosition, nextDirection);

				if(traversedInteriorPositions.count(nextPosition) == 0 && isBorderAdjacent(nextPosition, borderPositions))
				{
					// Algorithm will traverse in this untraversed direction from the start position.
					terminateTraversal = false;

					if(nextDirection != currDirection)
					{
						currDirection = nextDirection;
						// Only add position to polygon if direction changed.
						polygon.push_back(start);
					}

					break;
				}
				else
				{
					++nextDirection;
				}
			}

			if(terminateTraversal)
			{
				if (!sameLateralLineOrdered(polygon.front(), start, polygon.back()))
				{
					// Add start position as it was missed by prior steps.
					polygon.push_back(start);
				}

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

std::vector<sf::Vector2f> convertPolygon(const std::vector<sf::Vector2i>& gridPolygon)
{
	std::vector<sf::Vector2f> polygon;

	// Polygon is a single point. 
	if(gridPolygon.size() == 1)
	{
		const sf::Vector2i position = gridPolygon[0];
		const float left = position.x * GRID_SQUARE_SIZE;
		const float right = (position.x * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;
		const float top = (position.y * GRID_SQUARE_SIZE);
		const float bottom = (position.y * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;

		polygon.push_back(sf::Vector2f(left, bottom));
		polygon.push_back(sf::Vector2f(left, top));
		polygon.push_back(sf::Vector2f(right, top));
		polygon.push_back(sf::Vector2f(right, bottom));

		return polygon;
	}

	int i = 1;
	sf::Vector2i curr = gridPolygon[0];
	sf::Vector2i next = gridPolygon[1];
	Direction direction = getRelativeDirection(curr, next);

	do
	{
		curr = next;
		i = (i + 1) % gridPolygon.size();
		next = gridPolygon[i];
		Direction nextDirection = getRelativeDirection(curr, next);

		assert(direction != nextDirection);

		addPolygonTraversalPoints(polygon, curr, direction, nextDirection);

		direction = nextDirection;
	} 
	while (i - 1 != 0);

	return polygon;
}

void addPolygonTraversalPoints(std::vector<sf::Vector2f> &polygon, sf::Vector2i position, Direction direction, Direction newDirection)
{
	assert(direction != newDirection);

	if (direction == Direction::up)
	{
		switch (newDirection)
		{
		case Direction::left:
			polygon.push_back(getBottomLeftCoordinate(position));
			break;
		case Direction::right:
			polygon.push_back(getTopLeftCoordinate(position));
			break;
		case Direction::down:
			polygon.push_back(getTopLeftCoordinate(position));
			polygon.push_back(getTopRightCoordinate(position));
			break;
		}
	}
	else if (direction == Direction::right)
	{
		switch (newDirection)
		{
		case Direction::up:
			polygon.push_back(getTopLeftCoordinate(position));
			break;
		case Direction::down:
			polygon.push_back(getTopRightCoordinate(position));
			break;
		case Direction::left:
			polygon.push_back(getTopRightCoordinate(position));
			polygon.push_back(getBottomRightCoordinate(position));
			break;
		}
	}
	else if (direction == Direction::down)
	{
		switch (newDirection)
		{
		case Direction::right:
			polygon.push_back(getTopRightCoordinate(position));
			break;
		case Direction::left:
			polygon.push_back(getBottomRightCoordinate(position));
			break;
		case Direction::up:
			polygon.push_back(getBottomRightCoordinate(position));
			polygon.push_back(getBottomLeftCoordinate(position));
			break;
		}
	}
	else if (direction == Direction::left)
	{
		switch (newDirection)
		{
		case Direction::down:
			polygon.push_back(getBottomRightCoordinate(position));
			break;
		case Direction::up:
			polygon.push_back(getBottomLeftCoordinate(position));
			break;
		case Direction::right:
			polygon.push_back(getBottomLeftCoordinate(position));
			polygon.push_back(getTopLeftCoordinate(position));
			break;
		}
	}

}

std::unordered_set<sf::Vector2i, Vector2iHash> determineBorderPositions(const std::unordered_set<sf::Vector2i, Vector2iHash>& positions)
{
	std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;

	for (const sf::Vector2i& position : positions)
	{
		std::vector<sf::Vector2i> adjacencies;
		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				if (x == 0 && y == 0)
				{
					continue;
				}

				adjacencies.push_back({ position.x + x, position.y + y });
			}
		}

		for (const sf::Vector2i &adjacency : adjacencies)
		{
			if (positions.count(adjacency) == 0)
			{
				borderPositions.insert(position);
				break;
			}
		}
	}

	return borderPositions;
}

sf::Vector2f getTopLeftCoordinate(const sf::Vector2i& position)
{
	return { position.x * GRID_SQUARE_SIZE, position.y * GRID_SQUARE_SIZE };
}

sf::Vector2f getTopRightCoordinate(const sf::Vector2i& position)
{
	return { (position.x + 1) * GRID_SQUARE_SIZE, position.y * GRID_SQUARE_SIZE };
}

sf::Vector2f getBottomLeftCoordinate(const sf::Vector2i& position)
{
	return { position.x * GRID_SQUARE_SIZE, (position.y + 1) * GRID_SQUARE_SIZE };
}

sf::Vector2f getBottomRightCoordinate(const sf::Vector2i& position)
{
	return { (position.x + 1) * GRID_SQUARE_SIZE, (position.y + 1) * GRID_SQUARE_SIZE };
}

bool areLateral(const sf::Vector2i& p1, const sf::Vector2i& p2)
{
	return p1.x - p2.x == 0 || p1.y - p2.y == 0;
}

bool sameLateralLineOrdered(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3)
{
	bool xOrdered = (p1.x >= p2.x && p2.x >= p3.x) || (p1.x <= p2.x && p2.x <= p3.x);
	bool yOrdered = (p1.y >= p2.y && p2.y >= p3.y) || (p1.y <= p2.y && p2.y <= p3.y);
	bool xDifferent = (p1.x != p2.x) || (p2.x != p3.x);
	bool yDifferent = (p1.y != p2.y) || (p2.y != p3.y);
	return (xOrdered && !yDifferent) || (yOrdered && !xDifferent);
}
