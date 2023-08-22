#pragma once
#include "Map.h"
#include "IController.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
public:
	Game(std::string mapName);

	void update(); 
	void draw(sf::RenderWindow &window); 
	void handleInput(const sf::RenderWindow &window, sf::View &view); 

private: 
	Map map; 
	std::list<Player> players; 
	std::list<Player>::iterator currPlayer;
};

