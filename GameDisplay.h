#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "MapDisplay.h"
#include "GameUI.h"

class Estate;

class GameDisplay
{
public:
	GameDisplay(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>> &players);
	void draw(sf::RenderWindow &window) const;

private:
	const Game &game;
	const std::vector<std::unique_ptr<Player>> &players;
	MapDisplay MapDisplay;  

	void drawMilitaries(sf::RenderWindow &window) const;
	void drawPorts(sf::RenderWindow &window) const;
};

