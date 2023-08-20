#include "LandArmy.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "Player.h"
#include "LandArmy.h"
#include "TerritoryManager.h"
#include "MapMaker.h"
#include "InputUtility.h"
#include "TestRunner.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
	sf::View view = window.getDefaultView();
	InputUtility &InputUtility = InputUtility::getInstance();

	MapMaker mapMaker;
	mapMaker.load("test");

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			InputUtility.handleInputEvent(event);

			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		mapMaker.handleInput(window, view);

		InputUtility.update();

		window.clear();
		window.setView(view);

		mapMaker.draw(window);

		window.display();
	}

	mapMaker.save("test");

	TestRunner testRunner;
	testRunner.runTests();

	return 0;
}