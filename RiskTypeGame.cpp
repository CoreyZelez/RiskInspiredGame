#include "MapMaker.h"
#include "InputUtility.h"
#include "UnitTestRunner.h"
#include "Game.h"
#include "TextureManager.h"
#include "GameController.h"
#include "FontManager.h"
#include "Grid.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

int main()
{
	sf::RenderWindow testingWindow(sf::VideoMode(2560, 1440), "TESTING", sf::Style::Default);

	std::unordered_set<sf::Vector2i, Vector2iHash> positions;

	// Polygon 1. Intersecting line strips.
	for (int i = 32; i <= 44; ++i)
	{
		positions.insert({ i, 5 });
		positions.insert({ i, 6 });
		positions.insert({ i, 7 });
	}
	for (int i = 3; i <= 8; ++i)
	{
		positions.insert({ 35, i });
		positions.insert({ 36, i });
		positions.insert({ 37, i });
	}
	for (int i = 3; i <= 20; ++i)
	{
		positions.insert({ 44, i });
		positions.insert({ 45, i });
		positions.insert({ 46, i });
	}
	for (int i = 40; i <= 60; ++i)
	{
		positions.insert({ i, 15 });
		positions.insert({ i, 16 });
		positions.insert({ i, 17 });
		positions.insert({ i, 18 });
	}

	// Polygon 2. Rectangle.
	for (int x = 33; x <= 50; ++x)
	{
		for (int y = 38; y <= 42; ++y)
		{
			positions.insert({ x, y });
		}
	}
	
	// Polygon 3. Rigid edges.
	for (int x = 15; x <= 30; ++x)
	{
		for (int y = 8; y <= x - 1; ++y)
		{
			positions.insert({ x, y });
		}
	}

	Grid grid(positions);
	grid.setBorderColor(sf::Color::Red);
	grid.setInteriorColor(sf::Color::Black);
	grid.update();

	while (testingWindow.isOpen())
	{	
		// Clear window.
		testingWindow.clear(sf::Color(255, 255, 255));

		grid.draw(testingWindow);

		// Display game.
		testingWindow.display();
	}

	// TESTS
	UnitTestRunner unitTestRunner;
	unitTestRunner.runTests();

	bool mapEditorMode = false;

	sf::RenderWindow window(sf::VideoMode(2560, 1440), "Sovereign Chaos", sf::Style::Fullscreen);
	sf::View gameView = window.getDefaultView(); 
	sf::View UIView = window.getDefaultView();   

	UIView.zoom(3);
	InputUtility &InputUtility = InputUtility::getInstance();
	FontManager::getInstance(); 
	TextureManager::getInstance();  // IF NOT PUT HERE WE GET ERRORS.

	MapMaker mapMaker("empty");
	Game game("The Grand Continent");

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

	return 0;
}