#include "HashFunctions.h"
#include "Territory.h" 
#include "Grid.h"
#include <functional> 

size_t Vector2iHash::operator()(const sf::Vector2i& vector) const 
{
	std::hash<int> hasher;
	size_t seed = 0;
	seed ^= hasher(vector.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= hasher(vector.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

size_t PairTerritoryIntHash::operator()(const std::pair<const Territory*, int>& key) const 
{
	std::hash<int> hasher;
	size_t seed = 0;
	// Hash the begin position of the territory grid.
	sf::Vector2i position = *(key.first)->getGrid().getPositions().begin();
	seed ^= hasher(position.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= hasher(position.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	// Hash the int value of the key.
	seed ^= key.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

size_t Vector2fHash::operator()(const sf::Vector2f& vector) const
{
	std::hash<int> hasher;
	size_t seed = 1;
	seed ^= hasher(vector.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= hasher(vector.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

size_t GridHash::operator()(const Grid &grid) const
{
	return std::hash<int>{}(grid.getId());
}

std::size_t TerritoryTypeHash::operator()(TerritoryType& type) const
{
	return static_cast<std::size_t>(type);
}
