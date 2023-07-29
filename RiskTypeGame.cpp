#include "LandArmy.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "Player.h"
#include "LandArmy.h"
#include "TerritoryManager.h"
#include "MapMaker.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1400, 1000), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	MapMaker mapMaker;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			mapMaker.handleEvent(window, event);
		}
		window.clear();
		mapMaker.draw(window);
		window.display();
	}

	// TESTING
	Player player1;
	Player player2;
	LandTerritory land1;
	LandTerritory land2;
	Barony barony1(&player1, land1);
	Barony barony2(&player2, land2);
	std::shared_ptr<LandArmy> army1 = std::make_shared<LandArmy>(player1, &land1, 50);
	land1.putArmy(army1);
	std::shared_ptr<LandArmy> army2 = std::make_shared<LandArmy>(player2, &land2, 7);
	land2.putArmy(army2);

	std::cout << army1.get()->getStrength() << "  " << army2.get()->getStrength() << std::endl;
	army1.get()->move(&land2, 30);
	std::cout << army1.get()->getStrength() << "  " << army2.get()->getStrength() << std::endl;
	std::cout << land2.getArmy().get()->getStrength() << "  " << army2.get()->getStrength() << std::endl;

	//Barony
	return 0;
}