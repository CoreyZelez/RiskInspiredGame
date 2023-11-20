#pragma once
#include <SFML/Graphics.hpp>
#include "MapDisplay.h"
#include "GameUI.h"

class Estate;

class GameDisplay
{
public:
	GameDisplay(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>>& players);
	void draw(sf::RenderWindow &window) const;

private:

	void drawMilitaries(sf::RenderWindow &window) const;

	const Game &game;
	const std::vector<std::unique_ptr<Player>> &players;
	MapDisplay MapDisplay;  
};
