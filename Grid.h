#pragma once
#include "HashFunctions.h"
#include <unordered_set>
#include <SFML/Graphics.hpp>

class Grid
{
public:
	Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions);

private:
	static int currId;

	int id;
	const std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;  // Coordinates of border positions.
	sf::VertexArray vertices;
	sf::Vector2i center;  // Center coordinates in game world.
};



