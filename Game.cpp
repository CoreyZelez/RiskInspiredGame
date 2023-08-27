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
		std::unique_ptr<Player> player = std::make_unique<Player>(*this);
		barony.get()->initRuler(*player.get());
		players.emplace_back(std::move(player));
	}
}

void Game::update()
{
	while(currPlayer != players.end())
	{
		currPlayer->get()->handleTurn();
		// Waiting for user input to complete turn.
		if(!currPlayer->get()->getTurnOver())
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