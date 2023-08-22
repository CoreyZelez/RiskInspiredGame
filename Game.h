#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game(std::string mapName);

	void update();
	void draw(sf::RenderWindow &window);
	void handleInput(const sf::RenderWindow &window, sf::View &view);	

private:
	Map map;

};

