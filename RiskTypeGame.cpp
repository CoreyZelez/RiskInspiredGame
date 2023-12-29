#include "MapMaker.h"
#include "InputUtility.h"
#include "UnitTestRunner.h"
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
	sf::View gameView = window.getDefaultView();
	sf::View UIView = window.getDefaultView();

	InputUtility &InputUtility = InputUtility::getInstance();
	TextureManager::getInstance();  // IF NOT PUT HERE WE GET ERRORS IN TESTING CODE!!!
	FontManager::getInstance();

	MapMaker mapMaker("empty");

	Game game("debugmap");

	GameDisplay gameDisplay = game.createView();
	GameUI gameUI;
	GameController gameController(game, gameUI, gameDisplay);

	while(window.isOpen())
	{
		// Detect input events.
		sf::Event event;
		while(window.pollEvent(event))
		{
			InputUtility.handleInputEvent(event);
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Update game.
		if(!mapEditorMode)
		{
			game.update();
		}

		// Handle input.
		window.setView(gameView);
		if(mapEditorMode)
		{
			mapMaker.handleInput(window, gameView);
		}
		else
		{
			gameController.handleGameInput(window, gameView);
			window.setView(UIView);
			gameController.handleUIInput(window, UIView);
		}

		// Update input.
		InputUtility.update();

		// Clear window.
		window.clear(sf::Color(255, 255, 255));

		// Display game.
		window.setView(gameView);
		if(mapEditorMode)
		{
			mapMaker.draw(window);
		}
		else
		{
			gameDisplay.draw(window);
		}

		// Display UI.
		window.setView(UIView);
		if(!mapEditorMode)
		{
			gameUI.draw(window);
		}
		window.display();
	}

	mapMaker.save();

	UnitTestRunner unitTestRunner;
	unitTestRunner.runTests();

	return 0;
}