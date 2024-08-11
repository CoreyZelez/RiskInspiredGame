#include "MapMaker.h"
#include "InputUtility.h"
#include "UnitTestRunner.h"
#include "Game.h"
#include "TextureManager.h"
#include "GameController.h"
#include "FontManager.h"
#include "CompositeGrid.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	bool mapEditorMode = false;

	sf::RenderWindow window(sf::VideoMode(2560, 1440), "Sovereign Chaos", sf::Style::Fullscreen);
	sf::View gameView = window.getDefaultView(); 
	sf::View UIView = window.getDefaultView();   

	UIView.zoom(3);
	InputUtility &InputUtility = InputUtility::getInstance();
	FontManager::getInstance(); 
	TextureManager::getInstance();  // IF NOT PUT HERE WE GET ERRORS.

	MapMaker mapMaker("empty");
	Game game("Executioner's Ring");

	GameDisplay gameDisplay = game.createView();
	GameUI gameUI(UIView);
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
			gameUI.update();
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

	// TESTS
	UnitTestRunner unitTestRunner;
	//unitTestRunner.runTests();

	return 0;
}