#include "MapMaker.h"
#include "ChangeInstance.h"
#include "ReconcileBaronies.h"
#include <assert.h>
#include <iostream>

MapMaker::MapMaker(std::string mapName)
	: map(mapName), territoryMaker(map.getTerritoryManager()), estateMaker(map.getEstateManager())
{
	// Button for entering territory maker.
	std::unique_ptr<Command> enterTerritoryMaker = std::make_unique<ChangeInstance<MapMakerState>>(state, MapMakerState::territoryMode);
	sf::Vector2f position1(300, 300);
	sf::Vector2f size1(200, 70);
	buttons.emplace_back(std::make_unique<Button>(position1, size1, enterTerritoryMaker));

	// Button for entering estate maker.
	std::vector<std::unique_ptr<Command>> enterEstateMakerCommands;
	// Changes map maker state.
	enterEstateMakerCommands.emplace_back(std::make_unique<ChangeInstance<MapMakerState>>(state, MapMakerState::estateMode));
	// Generates and removes necessary baronies.
	enterEstateMakerCommands.emplace_back(std::make_unique<ReconcileBaronies>(map.getEstateManager(), map.getTerritoryManager().getLandTerritories()));
	sf::Vector2f position2(700, 300);
	sf::Vector2f size2(200, 70);
	buttons.emplace_back(std::make_unique<Button>(position2, size2, enterEstateMakerCommands));
}

void MapMaker::save()
{
	map.save();
}

void MapMaker::saveAs(std::string mapName) 
{
	map.saveAs(mapName);
}

void MapMaker::load(std::string mapName)
{
	map = Map("mapName");
}

void MapMaker::draw(sf::RenderWindow &window)
{
	switch(state)
	{
	case MapMakerState::none:
		drawButtons(window);
		break;
	case MapMakerState::territoryMode:
		territoryMaker.draw(window);
		break;
	case MapMakerState::estateMode:
		estateMaker.draw(window);
		break;
	}
}

void MapMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	switch(state)
	{
	case MapMakerState::none:
		handleButtonInput(window);
		break;
	case MapMakerState::territoryMode:
		territoryMaker.handleInput(window, view);
		break;
	case MapMakerState::estateMode:
		estateMaker.handleInput(window, view);
		break;
	}
}

void MapMaker::handleButtonInput(const sf::RenderWindow &window)
{
	for(auto &button : buttons)
	{
		button.get()->handleInput(window);
	}
}

void MapMaker::drawButtons(sf::RenderWindow &window)
{
	for(auto &button : buttons)
	{
		button.get()->draw(window);
	}
}
