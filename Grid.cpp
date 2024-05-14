#include "Grid.h"

int Grid::currId = 1;

Grid::Grid(const std::unordered_set<sf::Vector2i, Vector2iHash> &borderPositions)
	: id(currId++), borderPositions(borderPositions)
{
}
