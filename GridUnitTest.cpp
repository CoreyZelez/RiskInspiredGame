#include "GridUnitTest.h"
#include "Grid.h"
#include <assert.h>

void GridUnitTest::test()
{
	extractInteriorPolygonTest();
}

void GridUnitTest::extractInteriorPolygonTest() const
{
	std::vector<std::vector<sf::Vector2i>> polygons;
	std::unordered_set<sf::Vector2i, Vector2iHash> traversedInteriorPositions;
	// Construct a square.
	const int width = 10;
	assert(width > 5);  // Ensures test case remains simple.
	std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;
	for(int x = 0; x < width; ++x)
	{
		for(int y = 0; y < width; ++y)
		{
			if(x == 0 || x == width - 1 || y == 0 || y == width - 1)
			{
				borderPositions.insert(sf::Vector2i(x, y));
			}
		}
	}
	sf::Vector2i start1 = sf::Vector2i(1, width / 2);
	sf::Vector2i offset1 = sf::Vector2i(1, 0);

	// First operation tested.
	// Extracting interior square with start position not in a corner.
	extractInteriorPolygon(start1, offset1, polygons, borderPositions, traversedInteriorPositions);
}


