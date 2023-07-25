#include "LandArmy.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "Player.h"
#include "LandArmy.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape);
		window.display();
	}

	// TESTING
	Player player1;
	Player player2;
	LandTerritory land1;
	LandTerritory land2;
	Barony barony1(land1);
	Barony barony2(land2);
	std::shared_ptr<LandArmy> army1 = std::make_shared<LandArmy>(player1, &land1, 8);
	land1.putArmy(army1);
	std::shared_ptr<LandArmy> army2 = std::make_shared<LandArmy>(player2, &land2, 7);
	land2.putArmy(army2);


	std::cout << army1.get()->getStrength() << "  " << army2.get()->getStrength() << std::endl;
	army1.get()->move(&land2, 5);
	std::cout << army1.get()->getStrength() << "  " << army2.get()->getStrength() << std::endl;

	//Barony
	return 0;
}