#pragma once
#include <SFML/Graphics.hpp>

class Game;
class GameUI;

class GameUIController
{
public:
	GameUIController(Game &game, GameUI &gameUI);

	void handleInput(const sf::RenderWindow &window, sf::View &UIView);
	void handlePanelCreation(const sf::RenderWindow &window);
private:
	GameUI &gameUI;
	Game &game;
};

