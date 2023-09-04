#include "GameController.h"
#include "InputUtility.h"
#include "Game.h"
#include <iostream>

GameController::GameController(Game &game, GameView &gameView)
	: game(game), gameView(gameView)
{
}

void GameController::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForWindowView(view);

	handleInputForMapView();

	handleInputForGameView(window);

	// POTENTIAL RISK (MAYBE) FOR RACE CONDITIONS WHEN IMPLEMENTING MULTITHREADING.
	handleInputForHumanPlayer(window);

	inputClock.restart();
}

void GameController::handleInputForWindowView(sf::View &view)
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

void GameController::handleInputForMapView()
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle ending of human player turn.
	if(inputUtility.getKeyPressed(sf::Keyboard::F1))
	{
		gameView.setMapMode(MapMode::realm);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F2))
	{
		gameView.setMapMode(MapMode::county);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F3))
	{
		gameView.setMapMode(MapMode::duchy);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F4))
	{
		gameView.setMapMode(MapMode::kingdom);
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::F5))
	{
		gameView.setMapMode(MapMode::empire);
	}
}

void GameController::handleInputForGameView(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Toggles military display.
	if(inputUtility.getKeyPressed(sf::Keyboard::M))
	{
		game.changeDisplayMilitary();
	}

	// Display information about map estates.
	if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		const MapMode mapMode = gameView.getMapMode();
		switch(mapMode)
		{
		case MapMode::realm:
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
			const Realm *realm = game.getRealm(worldPos);
			if(realm != nullptr)
			{
				gameView.setLeftPanel(*realm);
			}
		}
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
