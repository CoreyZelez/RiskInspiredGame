#include "CompositeGridUnitTest.h"
#include "Grid.h"
#include "CompositeGrid.h"

void CompositeGridUnitTest::test()
{
	testSetColor();
}

void CompositeGridUnitTest::testSetColor() const
{
	std::unordered_set<sf::Vector2i, Vector2iHash> positions1;
	std::unordered_set<sf::Vector2i, Vector2iHash> positions2;
	
	for (int x = 0; x <= 10; ++x)
	{
		for (int y = 0; y <= 10; ++y)
		{
			positions1.insert({ x, y });
		}
	}
	
	for (int x = 20; x <= 30; ++x)
	{
		for (int y = 20; y <= 30; ++y)
		{
			positions2.insert({ x, y });
		}
	}
	
	Grid grid1(positions1);
	Grid grid2(positions2);
	
	CompositeGrid compositeGrid;
	
	compositeGrid.addGrid(grid1, sf::Color(sf::Color::White));
	compositeGrid.addGrid(grid2, sf::Color(sf::Color::White));

	compositeGrid.setColor(grid1.getId(), sf::Color::Red);
	compositeGrid.update();
}
