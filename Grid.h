#pragma once
#include "HashFunctions.h"
#include <unordered_set>
#include <SFML/Graphics.hpp>

enum class Direction;

class Grid
{
public:
	Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> &positions, const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions);

	Grid() = default;

	void update();

	void setBorderColor(const sf::Color &color);
	void setInteriorColor(const sf::Color &color);
	const sf::Color &getInteriorColor() const;

	int getId() const;
	bool sameId(const Grid &grid) const;

	const sf::VertexArray& getVertices() const;

	bool isAdjacent(const Grid &grid) const;

private:
	static int currId;

	int id;

	sf::Color borderColor;
	sf::Color interiorColor;

	const std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;  // Coordinates of border positions.

	sf::VertexArray vertices;  // Aggregation of interior and border vertices.
	sf::VertexArray borderVertices;
	sf::VertexArray interiorVertices;

	sf::Vector2i center;  // Center coordinates in game world.

	// Updates colors of border vertices.
	void updateBorderVertices();
	// Updates colors of interior vertices.
	void updateInteriorVertices();
};

// Returns true if position is not a border position and is adjacent to a border position.
bool isBorderAdjacent(const sf::Vector2i &position, 
	const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions, bool includeBorders = false);

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

