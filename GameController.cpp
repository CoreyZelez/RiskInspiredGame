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

	handleInputForMapDisplay(window);

	handleInputForGameDisplay(window);

	gameUIController.handleUICreation(window);

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

void GameController::handleInputForMapDisplay(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle ending of human player turn.
	if(inputUtility.getKeyPressed(sf::Keyboard::F1))
	{
		game.resetVassalViews();
		game.deselectSelectedRealm();
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
	else if(inputUtility.getKeyPressed(sf::Keyboard::E))
	{
		if(game.getMapMode() == MapMode::selectedRealm)
		{
			game.deselectSelectedRealm();
			game.setMapMode(MapMode::realm);
		}
		else
		{
			game.setMapMode(MapMode::selectedRealm);
			game.selectCurrPlayerRealm(true);
		}
	}
	else if(inputUtility.getKeyDown(sf::Keyboard::LControl))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		if(inputUtility.getButtonPressed(sf::Mouse::Left))
		{
			if(game.getMapMode() == MapMode::realm)
			{
				game.setVassalView(worldPos);
			}
		}
	}
	else if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		if(game.getMapMode() == MapMode::selectedRealm)
		{
			game.selectPlayerRealm(worldPos);
			if(!game.isSelectedRealm())
			{
				game.setMapMode(MapMode::realm);
			}
		}
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
