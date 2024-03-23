#include "Game.h"
#include "GameplaySettings.h"
#include <iostream>
#include <thread>
#include <vector>

Game::Game(std::string mapName)
	: Game(GameplaySettings(), mapName)
{
}

Game::Game(GameplaySettings gameplaySettings, std::string mapName)
	: gameplaySettings(gameplaySettings), map(mapName), nameGenerator("realms")
{
	generatePlayers();
}

void Game::generatePlayers()
{
	const int numHumans = 0;
	int humanCnt = 0; 
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

Realm* Game::getRealm(const sf::Vector2f &position)
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

void Game::setDiplomacyRealmColors()
{
	if(selectedDiplomacyPlayer == nullptr)
	{
		return;
	}

	// Set colors of player realms with relationship to selected diplomacy player.
	selectedDiplomacyPlayer->setDiplomacyColors(players);
}

void Game::deselectDiplomacyPlayer()
{
	// Reset all grid colors of players to default.
	if(selectedDiplomacyPlayer != nullptr)
	{
		for(auto &player : players)
		{
			player.get()->getRealm().setGridColorDefault();
		}
		selectedDiplomacyPlayer->getRealm().setGridColorDefault();
	}
	selectedDiplomacyPlayer = nullptr;
}

bool Game::isDiplomacyView() const
{
	return selectedDiplomacyPlayer != nullptr;
}

bool Game::currPlayerIsHuman() const
{
	assert(players.size() > 0);
	return players[currPlayer].get()->getIsHuman();
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

const GameplaySettings & Game::getGameplaySettings() const
{
	return gameplaySettings;
}

void Game::update()
{
	///////////////////////////////
	// Caps number of turns per update call. For testing maybe... 
	const int maxTurns = 4;
	double turnCnt = 0;
	///////////////////////////////

	// Iterate through all players, updating their realms vertex arrays if their realm changed.
	auto iter = players.begin();
	while(iter != players.end())
	{
		if(*iter != nullptr)
		{
			iter->get()->getRealm().updateGrid();
			++iter;
		}
	}

	// Waiting for human player to end their turn.
	if(humanPlayerTurn)
	{
		return;
	}

	// Iterate through players handling their turns until a human player is reached.
	while(turnCnt < maxTurns)
	{
		++turnCnt;
		++currPlayer;


		if(currPlayer == players.size())
		{
			currPlayer = 0;
		}

		// Reduce turnCnt to reduce time spend simulating vassal turns (due to drawing expense).
		if(players[currPlayer].get()->hasLiege())
		{
			turnCnt -= 0.8;
		}

		if(players[currPlayer].get()->checkGameOver())
		{
			if(selectedDiplomacyPlayer == players[currPlayer].get())
			{
				selectedDiplomacyPlayer = nullptr;
			}
			players[currPlayer].get()->handleGameOver();
			players.erase(players.begin() + currPlayer);
			--currPlayer;
			continue;
		}

		if(selectedDiplomacyPlayer == players[currPlayer].get())
		{
			setDiplomacyRealmColors();
		}
		players[currPlayer].get()->handleTurn();

		// Need to wait for user input to complete turn.
		if(players[currPlayer].get()->getIsHuman())
		{
			humanPlayerTurn = true;
			break;
		}
	}

	//// Iterate through all players, updating their realms vertex arrays if their realm changed and they are to be drawn.
	//iter = players.begin();
	//while(iter != players.end())
	//{
	//	if(*iter == nullptr)
	//	{
	//		continue;
	//	}
	//
	//	Player &player = *iter->get();
	//
	//	// Do not update realm grid if player has liege and vassal view is not active.
	//	if(player.getLiege() != nullptr && player.getLiege()->getRealm().getVassalView())
	//	{
	//		iter++;
	//		continue;
	//	}
	//
	//	// Update realm grid.
	//	player.getRealm().updateGrid();
	//
	//	iter++;
	//}

	updateGrids();
}

void Game::updateGrids()
{
	std::vector<std::thread> threads;

	// Iterate through all players, creating a thread for each
	auto iter = players.begin();
	while(iter != players.end())
	{
		if(*iter == nullptr)
		{
			iter++;
			continue;
		}

		Player &player = *iter->get();

		bool gridOutdated = player.getRealm().gridIsOutdated();
		bool gridDraw = player.getLiege() == nullptr || !player.getLiege()->getRealm().getVassalView();
		if(gridDraw && gridOutdated) 
		{
			threads.emplace_back([&player]() { player.getRealm().updateGrid(); });
		}

		iter++;
	}

	// Join all threads to wait for them to complete
	for(auto& thread : threads) 
	{
		thread.join();
	}
}

GameState Game::getState() const
{
	return state;
}

Player& Game::createPlayer()
{
	const std::string realmName = nameGenerator.generateRandomName();
	players.emplace_back(std::make_unique<Player>(*this, realmName));
	return *players.back().get();
}

void Game::setMapMode(MapMode mapMode)
{
	switch(mapMode)
	{
	case MapMode::terrain:
		map.getTerritoryManager().setDrawMode(TerritoryDrawMode::terrain);
		break;
	case MapMode::culture:
		map.getTerritoryManager().setDrawMode(TerritoryDrawMode::culture);
		break;
	case MapMode::prosperity:
		map.getTerritoryManager().setDrawMode(TerritoryDrawMode::prosperity);
		break;
	}

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
			selectedDiplomacyPlayer = players[currPlayer].get();
			setDiplomacyRealmColors();
			return;
		}
	}

	deselectDiplomacyPlayer();
}

Realm& Game::getCurrPlayerRealm()
{
	return players[currPlayer].get()->getRealm();
}

Player& Game::getCurrPlayer()
{
	return *players[currPlayer].get();
}

void Game::selectDiplomacyPlayer(const sf::Vector2f &position)
{
	const bool considerVassalView = true;
	for(auto &player : players)
	{
		Realm &realm = player.get()->getRealm(); 
		if(realm.containsPosition(position, considerVassalView))
		{
			deselectDiplomacyPlayer();
			selectedDiplomacyPlayer = player.get();
			setDiplomacyRealmColors();
			return;
		}
	}

	deselectDiplomacyPlayer();
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