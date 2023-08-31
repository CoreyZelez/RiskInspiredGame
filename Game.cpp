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

		///if(!currPlayer->get()->getIsHuman())
		///{
		///	return;
		///}

		currPlayer->get()->handleTurn();

		// Need to wait for user input to complete turn.
		if(currPlayer->get()->getIsHuman())
		{
			humanPlayerTurn = true;
			return;
		}
	}
}

GameState Game::getState() const
{
	return state;
}

bool Game::getDisplayMilitary() const
{
	// Militaries not selected in realm select mode.
	if(state == GameState::realmSelectMode)
	{
		return false;
	}

	return displayMilitary;
}

void Game::selectMilitary(sf::Vector2f position)
{
	if(!humanPlayerTurn)
	{
		assert(selectedMilitary == nullptr);
		return;
	}
	assert(currPlayer->get()->getIsHuman());

	// Select military.
	selectedMilitary = currPlayer->get()->getMilitaryManager().getMilitary(position);

	// Set game state.
	if(selectedMilitary != nullptr)
	{
		// Set selectedStrength to total strength of military.
		selectedStrength = selectedMilitary->getStrength();

		state = GameState::militarySelected;
	}
	else
	{
		state = GameState::idle;
	}
}

void Game::moveSelectedMilitary(sf::Vector2f position)
{
	Territory *territory = map.getTerritoryManager().getTerritory(position);

	// Deselect military.
	if(territory == nullptr)
	{
		state = GameState::idle;
		selectedMilitary = nullptr;
		return;
	}

	// Move the military.
	selectedMilitary->move(*territory, selectedStrength);
	// Deselect military.
	state = GameState::idle;
	selectedMilitary = nullptr;
}

void Game::endHumanPlayerTurn()
{
	if(humanPlayerTurn)
	{
		humanPlayerTurn = false;
	}
}

void Game::changeDisplayMilitary()
{
	displayMilitary = !displayMilitary;
}

GameView Game::createView() const
{
	return GameView(*this, map, players);
}