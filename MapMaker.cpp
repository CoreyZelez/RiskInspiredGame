#include "MapMaker.h"
#include "ChangeInstance.h"
#include <assert.h>
#include <iostream>

MapMaker::MapMaker()
	: territoryMaker(map.getTerritoryManager())
{
	std::unique_ptr<Command> enterTerritoryMaker = std::make_unique<ChangeInstance<MapMakerState>>(state, MapMakerState::territoryMode);
	sf::Vector2f position(300, 300);
	sf::Vector2f size(200, 70);
	buttons.push_back(Button(position, size, std::move(enterTerritoryMaker)));
}

void MapMaker::save(std::string name) const
{
	map.save(name);
}

void MapMaker::load(std::string name)
{
	map.load(name);
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
		estateMaker.handleInput(window);
		break;
	}
	
}

void MapMaker::handleButtonInput(const sf::RenderWindow &window)
{
	for(Button &button : buttons)
	{
		button.handleInput(window);
	}
}

void MapMaker::drawButtons(sf::RenderWindow &window)
{
	for(Button &button : buttons)
	{
		button.draw(window);
	}
}
