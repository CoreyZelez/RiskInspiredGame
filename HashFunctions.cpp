#include "HashFunctions.h"
#include "Territory.h" // Include the header where specializations are declared
#include <functional> // Include necessary headers

// Implement Vector2iHash
size_t Vector2iHash::operator()(const sf::Vector2i& vector) const 
{
	std::hash<int> hasher;
	size_t seed = 0;
	seed ^= hasher(vector.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= hasher(vector.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

// Implement PairTerritoryIntHash
size_t PairTerritoryIntHash::operator()(const std::pair<const Territory*, int>& key) const {
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


