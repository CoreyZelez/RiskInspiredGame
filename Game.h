#pragma once
#include "Map.h"
#include "Player.h"
#include "GameView.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
public:
	Game(std::string mapName);

	void update(); 

	void endHumanPlayerTurn();

	// Creates a view for the game.
	GameView createView() const;

private: 
	void generatePlayers();  // Creates and assigns 1 player per barony.

	bool humanPlayerTurn = false;  // Specifies that current turn is human player. Game waits for input.

	Map map; 
	std::vector<std::unique_ptr<Player>> players; 
	std::vector<std::unique_ptr<Player>>::iterator currPlayer;
};

