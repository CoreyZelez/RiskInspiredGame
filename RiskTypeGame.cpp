#include "MapMaker.h"
#include "InputUtility.h"
#include "TestRunner.h"
#include "Game.h"
#include "TextureManager.h"
#include "GameController.h"
#include "MilitaryManager.h"
#include "FontManager.h"
#include "LandArmy.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	bool mapEditorMode = false;

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
	sf::View view = window.getDefaultView();
	InputUtility &InputUtility = InputUtility::getInstance();
	TextureManager::getInstance();  // IF NOT PUT HERE WE GET ERRORS IN TESTING CODE!!!
	FontManager::getInstance();

	MapMaker mapMaker("test");

	Game game("test");
	GameView gameView = game.createView();
	GameController gameController(game, gameView);

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

		game.update();

		if(mapEditorMode)
		{
			mapMaker.handleInput(window, view);
		}
		else
		{
			gameController.handleInput(window, view);
		}
		InputUtility.update();
		window.clear(sf::Color(255, 255, 255));
		window.setView(view);
		if(mapEditorMode)
		{
			mapMaker.draw(window);
		}
		else
		{
			gameView.draw(window);
		}
		window.display();
	}

	mapMaker.save();

	TestRunner testRunner;
	testRunner.runTests();

	return 0;
}