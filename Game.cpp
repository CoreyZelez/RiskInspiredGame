#include "Game.h"

Game::Game(std::string mapName)
	: map(mapName)
{
	generatePlayers();
	currPlayer = players.begin();
}


void Game::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	
}

void Game::generatePlayers()
{
	for(auto &barony : map.getEstateManager().getBaronies())
	{
		Player player(*this);
		player.getRealm().addFief(barony.get());
		players.push_back(player);
	}
}

void Game::update()
{
	while(currPlayer != players.end())
	{
		currPlayer->handleTurn();
		// Waiting for user input to complete turn.
		if(currPlayer->getAwaitingUserInput())
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

void Game::draw(sf::RenderWindow &window)
{
	map.drawTerritoryMap(window);
}
