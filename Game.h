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
	void generatePlayers();  // Creates and assigns 1 player per barony.

	Map map; 
	std::list<Player> players; 
	std::list<Player>::iterator currPlayer;
};

