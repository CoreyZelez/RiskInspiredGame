#pragma once
#include "GameUIController.h"
#include <SFML/Graphics.hpp>

class Game;
class GameDisplay;

class GameController
{
public:
	GameController(Game &game, GameUI &gameUI, GameDisplay &GameDisplay);

	void handleGameInput(const sf::RenderWindow &window, sf::View &gameView);
	void handleUIInput(const sf::RenderWindow &window, sf::View &uiView);

private:
	void handleInputForGameView(sf::View &gameView);
	void handleInputForMapDisplay();
	void handleInputForGameDisplay(const sf::RenderWindow &window);
	void handleInputForHumanPlayer(const sf::RenderWindow &window);

	GameUIController gameUIController;
	Game &game;
	GameDisplay &gameDisplay;
	sf::Clock inputClock;  // Clock tracking time since last time input handled.
};
