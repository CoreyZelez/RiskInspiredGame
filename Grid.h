#pragma once
#include "HashFunctions.h"
#include <unordered_set>
#include <SFML/Graphics.hpp>

const float GRID_SQUARE_SIZE = 30.0f;  // Width of each grid square.

enum class Direction;

class Grid
{
public:
	Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> &positions);

	Grid() = default;

	void update();

	void draw(sf::RenderWindow& window) const;

	void setBorderColor(const sf::Color &color);
	const std::unordered_set<sf::Vector2i, Vector2iHash>& getBorderPositions() const;
	void setInteriorColor(const sf::Color &color);
	const sf::Color &getInteriorColor() const;

	int getId() const;
	bool sameId(const Grid &grid) const;

	const sf::VertexArray& getVertices() const;

	bool containsBorderPosition(const sf::Vector2i& position) const;
	bool containsPosition(const sf::Vector2i& position) const;
	bool isAdjacent(const Grid &grid) const;
	bool positionAdjacentToBorder(const sf::Vector2i &position) const;

private:
	static int currId;

	int id;

	sf::Color borderColor = sf::Color::Black;
	sf::Color interiorColor = sf::Color::White;

	const std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;  // Grid coordinates of border positions.

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// INSTEAD HOLD REFERENCE TO A "POSITION STORAGE" CLASS. IT MAPS ID TO POSITIONS. THAT WAY DONT HAVE TO COPY.
	const std::unordered_set<sf::Vector2i, Vector2iHash> positions;  // All grid coordinates.
	///////////////////////////////////////////////////////////////////

	sf::VertexArray vertices;  // Aggregation of interior and border vertices.
	sf::VertexArray borderVertices;
	sf::VertexArray interiorVertices;

	sf::Vector2i center;  // Center coordinates in game world.

	void initBorderVertices(const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions); 
	void initInteriorVertices(const std::unordered_set<sf::Vector2i, Vector2iHash>& positions,
			const std::unordered_set<sf::Vector2i, Vector2iHash>& borderPositions);

	// Updates colors of border vertices.
	void updateBorderVertices();
	// Updates colors of interior vertices.
	void updateInteriorVertices();
	// Combines constructs vertex array of all interior and border vertices.
	void aggregateVertices();
};

// Returns true if position is not a border position and is adjacent to a border position.
bool isBorderAdjacent(const sf::Vector2i &position, 
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions, bool includeDiagonals = true, bool includeBorders = false);

// Returns true if adjacent points to position are border positions
bool isIsolated(const sf::Vector2i &position, const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions);

// Returns the initial direction for traversal from start position such that the interior polygon is traversed clockwise.
// Specify the offset of the start position from the border position.
Direction initialClockwiseDirection(const sf::Vector2i &offsetFromBorder);

void extractInteriorPolygon(const sf::Vector2i &start, const sf::Vector2i &offsetFromBorder,
	std::vector<std::vector<sf::Vector2i>> &polygons,
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions,
	std::unordered_set<sf::Vector2i, Vector2iHash> &traversedInteriorPositions);

std::vector<std::vector<sf::Vector2i>> extractInteriorPolygons(const std::unordered_set<sf::Vector2i, Vector2iHash> &positions, 
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions);

// Converts a polygon which has vertices specified in terms of grid coordinates to polygon with vertices specified in
// terms of world float coordinates. Parameter polygon must have vertices in clockwise order.
std::vector<sf::Vector2f> convertPolygon(const std::vector<sf::Vector2i> &gridPolygon);

// Adds world points to polygon based on grid points of polygon.
void addPolygonTraversalPoints(std::vector<sf::Vector2f>& polygon, sf::Vector2i point, Direction direction, Direction newDirection);

// Determines border positions amongs a set of positions.
std::unordered_set<sf::Vector2i, Vector2iHash> determineBorderPositions(const std::unordered_set<sf::Vector2i, Vector2iHash>& positions);

// Returns the top left world coordinate of a grid cell.
sf::Vector2f getTopLeftCoordinate(const sf::Vector2i & position);
// Returns the top right world coordinate of a grid cell.
sf::Vector2f getTopRightCoordinate(const sf::Vector2i& position);
// Returns the bottom left world coordinate of a grid cell.
sf::Vector2f getBottomLeftCoordinate(const sf::Vector2i& position);
// Returns the bottom right world coordinate of a grid cell.
sf::Vector2f getBottomRightCoordinate(const sf::Vector2i& position);

// Returns true if points are displaced from eachother either vertically or horizontally.
bool areLateral(const sf::Vector2i &p1, const sf::Vector2i &p2);

// Returns true if the three points lie on the same vertical or horizontal line in the specified order.
bool sameLateralLineOrdered(const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3);




