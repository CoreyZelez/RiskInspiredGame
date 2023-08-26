#include "Game.h"
#include <iostream>

Game::Game(std::string mapName)
	: map(mapName)
{
	generatePlayers();
	currPlayer = players.begin();
}

void Game::generatePlayers()
{
	for(auto &barony : map.getEstateManager().getBaronies())
	{
		Player player(*this);
		barony.get()->initRuler(player);
		players.push_back(player);
	}
}

void Game::update()
{

	while(currPlayer != players.end())
	{
		currPlayer->handleTurn();
		// Waiting for user input to complete turn.
		if(!currPlayer->getTurnOver())
		{
			return;
		}
		++currPlayer;
	}

	if(currPlayer == players.end())
	{
		// removeEliminatedPlayers()  
		currPlayer = players.begin();
	}
}

GameView Game::createView() const
{
	return GameView(map, players);
}