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
	int humanCnt = 0; // Used temporarily for testing.
	const int numHumans = 0;
	for(auto &barony : map.getEstateManager().getBaronies())
	{
		std::unique_ptr<Player> player = std::make_unique<Player>(*this);
		barony.get()->setRuler(player.get());

		// Provide starting yields for each player.
		for(int i = 0; i < 20; ++i)
		{
			player.get()->getRealm().getEstateManager().handleFiefYields();
		}

		if(humanCnt < numHumans)
		{
			++humanCnt;
			player.get()->setHuman();
		}
		players.emplace_back(std::move(player));
	}
}

Realm* Game::getRealm(const sf::Vector2f & position)
{
	for(auto &player : players)
	{
		Realm &realm = player.get()->getRealm();
		if(realm.getEstateManager().containsPosition(position))
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
		selectedRealm->getEstateManager().setGridColorDefault();
	}
	selectedRealm = nullptr;
}

bool Game::isSelectedRealm() const
{
	return selectedRealm != nullptr;
}

void Game::update()
{
	///////////////////////////////
	// Caps number of turns per update call. For testing. 
	const int maxTurns = 100;
	int turnCnt = 0;
	///////////////////////////////

	// Waiting for human player to end their turn.
	if(humanPlayerTurn)
	{
		return;
	}

	// Iterate through players handling their turns until a human player is reached.
	while(currPlayer != players.end() && turnCnt < maxTurns)
	{
		++turnCnt;
		++currPlayer;

		if(currPlayer == players.end())
		{
			currPlayer = players.begin();
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

GameState Game::getState() const
{
	return state;
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
	assert(currPlayer->get()->getIsHuman());

	// Select military.
	selectedMilitary = currPlayer->get()->getMilitaryManager().getMilitary(position);

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

const Realm* Game::getRealm(const sf::Vector2f &position) const
{
	for(const auto &player : players)
	{
		const Realm &realm = player.get()->getRealm();
		if(realm.getEstateManager().containsPosition(position))
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
	if(*currPlayer != nullptr)
	{
		if(humanOnly && humanPlayerTurn)
		{
			assert(currPlayer->get()->getIsHuman());
			selectedRealm = &currPlayer->get()->getRealm();
			selectedRealm->getEstateManager().setGridColor(sf::Color::Yellow);
			return;
		}
		else
		{
			selectedRealm =  &currPlayer->get()->getRealm();
			selectedRealm->getEstateManager().setGridColor(sf::Color::Yellow);
			return;
		}
	}

	deselectSelectedRealm();
}

void Game::selectPlayerRealm(const sf::Vector2f & position)
{
	Realm *realm = getRealm(position);
	if(realm != nullptr)
	{
		deselectSelectedRealm();
		selectedRealm = realm;
		selectedRealm->getEstateManager().setGridColor(sf::Color::Yellow);
	}
	else
	{
		deselectSelectedRealm();
	}
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