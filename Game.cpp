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
	bool firstHuman = false; // Used temporarily for testing.

	for(auto &barony : map.getEstateManager().getBaronies())
	{
		AIPersonality tempPersonality;  // Personality for testing.
		std::unique_ptr<Player> player = std::make_unique<Player>(*this, tempPersonality);
		barony.get()->initRuler(*player.get());
		if(!firstHuman)
		{
			firstHuman = true;
			player.get()->setHuman();
		}

		players.emplace_back(std::move(player));
	}
}

void Game::update()
{
	// Waiting for human player to end their turn.
	if(humanPlayerTurn)
	{
		return;
	}

	// Iterate through players handling their turns until a human player is reached.
	while(currPlayer != players.end())
	{
		if(currPlayer == players.end())
		{
			currPlayer = players.begin();
		}
		else
		{
			++currPlayer;
			if(currPlayer == players.end())
			{
				currPlayer = players.begin();
			}
		}

		currPlayer->get()->handleTurn();

		// Need to wait for user input to complete turn.
		if(currPlayer->get()->getIsHuman())
		{
			humanPlayerTurn = true;
			return;
		}
	}
}

void Game::endHumanPlayerTurn()
{
	if(humanPlayerTurn)
	{
		humanPlayerTurn = false;
	}
}

GameView Game::createView() const
{
	return GameView(map, players);
}