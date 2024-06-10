#pragma once
#include "HashFunctions.h"
#include <unordered_set>
#include <SFML/Graphics.hpp>

enum class Direction;

class Grid
{
public:
	Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> & positions, const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions);

	bool sameId(const Grid &grid) const;
	bool isAdjacent(const Grid &grid) const;

private:
	static int currId;

	int id;
	const std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;  // Coordinates of border positions.
	sf::VertexArray vertices;
	sf::Vector2i center;  // Center coordinates in game world.
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


