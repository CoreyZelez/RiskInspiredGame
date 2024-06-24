#include "GridUnitTest.h"
#include "Grid.h"
#include <assert.h>

void GridUnitTest::test()
{
	extractInteriorPolygonTest1();
	extractInteriorPolygonTest2();
	extractInteriorPolygonsTest1();
	constructorTest1();
}

void GridUnitTest::extractInteriorPolygonTest1() const
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

void GridUnitTest::extractInteriorPolygonTest2() const
{
	std::vector<std::vector<sf::Vector2i>> polygons;
	std::unordered_set<sf::Vector2i, Vector2iHash> traversedInteriorPositions;

	std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;
	for(int x = 0; x <= 10; ++x)
	{
		borderPositions.insert(sf::Vector2i(x, 0));
		borderPositions.insert(sf::Vector2i(x, 2));
	}

	borderPositions.insert(sf::Vector2i(0, 1));
	borderPositions.insert(sf::Vector2i(12, 2));
	borderPositions.insert(sf::Vector2i(12, 1));
	borderPositions.insert(sf::Vector2i(12, 0));

	for(int y = 3; y <= 10; ++y)
	{
		borderPositions.insert(sf::Vector2i(10, y));
		borderPositions.insert(sf::Vector2i(12, y));
	}

	borderPositions.insert(sf::Vector2i(11, 10));

	for(int y = 3; y <= 10; ++y)
	{
		borderPositions.insert(sf::Vector2i(10, y));
		borderPositions.insert(sf::Vector2i(12, y));
	}

	for(int y = -1; y >= -10; --y)
	{
		borderPositions.insert(sf::Vector2i(10, y));
		borderPositions.insert(sf::Vector2i(12, y));
	}

	borderPositions.insert(sf::Vector2i(11, -10));

	// Start at the intersection of the horzontal and vertical section of the polygon.
	sf::Vector2i start1 = sf::Vector2i(11, 1);
	sf::Vector2i offset1 = sf::Vector2i(-1, 0);

	// First operation tested.
	// Extracting interior square with start position not in a corner.
	extractInteriorPolygon(start1, offset1, polygons, borderPositions, traversedInteriorPositions);
}

void GridUnitTest::extractInteriorPolygonsTest1() const
{
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;

	// Points for first polygon.
	for(int x = 0; x <= 10; ++x)
	{
		for(int y = 0; y <= 10; ++y)
		{
			positions.insert(sf::Vector2i(x, y));
		}
	}

	// Points for second polygon.
	for(int x = -20; x <= -15; ++x)
	{
		for(int y = -20; y <= -15; ++y)
		{
			positions.insert(sf::Vector2i(x, y));
		}
	}

	// Points for third polygon. Shaped like a line.
	for(int x = 30; x <= 40; ++x)
	{
		positions.insert(sf::Vector2i(x, 30));
		positions.insert(sf::Vector2i(x, 31));
		positions.insert(sf::Vector2i(x, 29));
	}

	std::vector<std::vector<sf::Vector2i>> polygons = extractInteriorPolygons(positions, determineBorderPositions(positions));
}

void GridUnitTest::constructorTest1() const
{
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;

	// Polygon 1. Intersecting line strips.
	for (int i = 0; i <= 30; ++i)
	{
		positions.insert({ i, 0 });
		positions.insert({ i, 1 });
		positions.insert({ i, 2 });
	}
	for (int i = -5; i <= 5; ++i)
	{
		positions.insert({ 10, i });
		positions.insert({ 11, i });
		positions.insert({ 12, i });
	}
	for (int i = -10; i <= 10; ++i)
	{
		positions.insert({ 20, i });
		positions.insert({ 21, i });
		positions.insert({ 22, i });
	}

	// Polygon 2. Large rectangle.
	for (int x = 60; x <= 2000; ++x)
	{
		for (int y = -100; y <= 800; ++y)
		{
			positions.insert({ x, y });
		}
	}

	// Polygon 3. Rigid edges.
	for (int x = -1000; x <= -800; ++x)
	{
		for (int y = -1200; y <= x; ++y)
		{
			positions.insert({ x, y });
		}
	}
	
	Grid grid(positions);
}


