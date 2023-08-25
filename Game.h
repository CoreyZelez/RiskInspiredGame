#pragma once
#include "Map.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
public:
	Game(std::string mapName);

	void draw(sf::RenderWindow &window);
	void update(); 
	void handleInput(const sf::RenderWindow &window, sf::View &view); 

private: 
	Map map; 
	std::list<Player> players; 
	std::list<Player>::iterator currPlayer;
};

