#include "Game.h"
#include <iostream>

Game::Game(std::string mapName)
	: map(mapName)
{
	generatePlayers();
}

void Game::generatePlayers()
{
	int humanCnt = 0; // Used temporarily for testing.
	const int numHumans = 0;
	for(auto &barony : map.getEstateManager().getBaronies())
	{
		Player &player = createPlayer(); 

		barony.get()->setOwnership(&player);

		// Provide starting yields for each player.
		for(int i = 0; i < 20; ++i)
		{
			player.getRealm().handleMilitaryYields();
		}

		if(humanCnt < numHumans)
		{
			++humanCnt;
			player.setHuman();
		}
	}
}

Realm* Game::getRealm(const sf::Vector2f & position)
{
	for(auto &player : players)
	{
		Realm &realm = player.get()->getRealm();
		if(realm.containsPosition(position))
		{
			return &realm;
		}
	}
	return nullptr;
}

void Game::deselectSelectedRealm()
{
	if(selectedRealm != nullptr)
	{
		selectedRealm->setGridColorDefault();
	}
	selectedRealm = nullptr;
}

bool Game::isSelectedRealm() const
{
	return selectedRealm != nullptr;
}

void Game::setVassalView(const sf::Vector2f & position)
{
	for(const auto &player : players)
	{
		Realm &realm = player.get()->getRealm();
		const bool vassalView = true;
		if(realm.containsPosition(position, vassalView))
		{
			realm.setVassalView(true);
			return;
		}
	}
}

void Game::resetVassalViews()
{
	for(const auto &player : players)
	{
		Realm &realm = player.get()->getRealm();
		const bool vassalView = true;
		realm.setVassalView(false);
	}
}

void Game::update()
{
	///////////////////////////////
	// Caps number of turns per update call. For testing. 
	const int maxTurns = 30;
	int turnCnt = 0;
	///////////////////////////////

	// Waiting for human player to end their turn.
	if(humanPlayerTurn)
	{
		// Iterate through all players, updating their realms vertex arrays if thier realm changed.
		for(auto &player : players)
		{
			player.get()->getRealm().updateGrid();
		}
		return;
	}

	// Iterate through players handling their turns until a human player is reached.
	while(currPlayer != players.size() && turnCnt < maxTurns)
	{
		// We test this here rather than the loop condition to ensure players are
		// never removed from the players vector during iteration.
		assert(currPlayer < players.size());

		++turnCnt;
		++currPlayer;

		if(currPlayer == players.size())
		{
			currPlayer = 0;
		}

		players[currPlayer].get()->handleTurn();

		// Need to wait for user input to complete turn.
		if(players[currPlayer].get()->getIsHuman())
		{
			humanPlayerTurn = true;
			break;
		}
	}

	// Iterate through all players, updating their realms vertex arrays if thier realm changed.
	for(auto &player : players)
	{
		player.get()->getRealm().updateGrid();
    }
}

GameState Game::getState() const
{
	return state;
}

Player& Game::createPlayer()
{
	players.emplace_back(std::make_unique<Player>(*this));
	return *players.back().get();
}

void Game::setMapMode(MapMode mapMode)
{
	this->mapMode = mapMode;
}

MapMode Game::getMapMode() const
{
	return mapMode;
}

const MilitaryForce& Game::getSelectedMilitary() const
{
	assert(selectedMilitary != nullptr);
	return *selectedMilitary;
}

unsigned int& Game::getSelectedStrength()
{
	return selectedStrength;
}

void Game::selectMilitary(sf::Vector2f position)
{
	if(!humanPlayerTurn)
	{
		assert(selectedMilitary == nullptr);
		return;
	}
	assert(players[currPlayer].get()->getIsHuman());

	// Select military.
	selectedMilitary = players[currPlayer].get()->getMilitaryManager().getMilitary(position);

	// Set game state.
	if(selectedMilitary != nullptr)
	{
		assert(selectedMilitary->getTotalStrength() > 0);

		// Set selectedStrength to total strength of military.
		selectedStrength = selectedMilitary->getTotalStrength();

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

const Map& Game::getMap() const
{
	return map;
}

const Realm* Game::getRealm(const sf::Vector2f &position, bool considerVassalView) const
{
	for(const auto &player : players)
	{
		const Realm &realm = player.get()->getRealm();
		if(realm.containsPosition(position, considerVassalView))
		{
			return &realm;
		}
	}
	return nullptr;
}

const Estate* Game::getEstate(const sf::Vector2f & position, Title title)
{
	const EstateManager &estateManager = map.getEstateManager();
	return estateManager.getEstate(position, title);
}

void Game::selectCurrPlayerRealm(bool humanOnly)
{
	if(currPlayer != players.size())
	{
		if(humanOnly && humanPlayerTurn)
		{
			assert(players[currPlayer].get()->getIsHuman());
			selectedRealm = &players[currPlayer].get()->getRealm();
			selectedRealm->setGridColor(sf::Color::Yellow);
			return;
		}
		else
		{
			selectedRealm =  &players[currPlayer].get()->getRealm();
			selectedRealm->setGridColor(sf::Color::Yellow);
			return;
		}
	}

	deselectSelectedRealm();
}

void Game::selectPlayerRealm(const sf::Vector2f &position)
{
	const bool considerVassalView = true;
	for(auto &player : players)
	{
		Realm &realm = player.get()->getRealm(); 
		if(realm.containsPosition(position, considerVassalView))
		{
			deselectSelectedRealm();
			selectedRealm = &realm;
			selectedRealm->setGridColor(sf::Color::Yellow);
			return;
		}
	}

	deselectSelectedRealm();
}

void Game::endHumanPlayerTurn()
{
	if(humanPlayerTurn)
	{
		humanPlayerTurn = false;
		state = GameState::idle;
		selectedMilitary = nullptr;
	}
}

void Game::changeDisplayMilitary()
{
	displayOptions.displayMilitaries = !displayOptions.displayMilitaries;
}

DisplayOptions Game::getDisplayOptions() const
{
	return displayOptions;
}

GameDisplay Game::createView() const
{
	return GameDisplay(*this, map, players);
}