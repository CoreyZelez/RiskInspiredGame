#include "GameController.h"
#include "InputUtility.h"
#include "GameDisplay.h"
#include "Game.h"
#include <iostream>

GameController::GameController(Game &game, GameUI &gameUI, GameDisplay &gameDisplay)
	: game(game), gameDisplay(gameDisplay), gameUIController(game, gameUI)
{
}

void GameController::handleUIInput(const sf::RenderWindow &window, sf::View &UIView)
{
	gameUIController.handleInput(window, UIView);
}

void GameController::handleGameInput(const sf::RenderWindow &window, sf::View &gameView)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForGameView(gameView);

	handleInputForMapDisplay();

	handleInputForGameDisplay(window);

	gameUIController.handlePanelCreation(window);

	// POTENTIAL RISK (MAYBE) FOR RACE CONDITIONS WHEN IMPLEMENTING MULTITHREADING.
	handleInputForHumanPlayer(window);

	inputClock.restart();
}

void GameController::handleInputForGameView(sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle view movement.
	const float speed = 3000.0f;
	const float effectiveSpeed = speed * inputClock.getElapsedTime().asSeconds();
	if(inputUtility.getKeyDown(sf::Keyboard::W))
	{
		view.move(0, -effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::A))
	{
		view.move(-effectiveSpeed, 0);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::S))
	{
		view.move(0, effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::D))
	{
		view.move(effectiveSpeed, 0);
	}

	// Handle view zoom.
	const float zoom = 0.1f;
	if(inputUtility.getMouseScrollDirection() == 1)
	{
		view.zoom(1 - zoom);
	}
	else if(inputUtility.getMouseScrollDirection() == -1)
	{
		view.zoom(1 + zoom);
	}
}

void GameController::handleInputForMapDisplay()
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle ending of human player turn.
	if(inputUtility.getKeyPressed(sf::Keyboard::F1))
	{
		game.setMapMode(MapMode::realm);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F2))
	{
		game.setMapMode(MapMode::county);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F3))
	{
		game.setMapMode(MapMode::duchy);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F4))
	{
		game.setMapMode(MapMode::kingdom);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F5))
	{
		game.setMapMode(MapMode::empire);
	}
}

void GameController::handleInputForGameDisplay(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Toggles military display.
	if(inputUtility.getKeyPressed(sf::Keyboard::M))
	{
		game.changeDisplayMilitary();
	}
}

void GameController::handleInputForHumanPlayer(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle ending of human player turn.
	if(inputUtility.getKeyPressed(sf::Keyboard::Space))
	{
		game.endHumanPlayerTurn();
	}

	if(inputUtility.getButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		if(game.getState() == GameState::idle)
		{
			game.selectMilitary(worldPos);
		}
		else if(game.getState() == GameState::militarySelected)
		{
			game.moveSelectedMilitary(worldPos);
		}
	}
}
