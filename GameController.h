#pragma once
#include <SFML/Graphics.hpp>

class Game;
class GameView;

class GameController
{
public:
	GameController(Game &game, GameView &gameUI);

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	void handleInputForWindowView(sf::View &view);
	void handleInputForMapView();
	void handleInputForGameView();
	void handleInputForHumanPlayer(const sf::RenderWindow &window);

	Game &game;
	GameView &gameView;
	sf::Clock inputClock;  // Clock tracking time since last time input handled.
};
