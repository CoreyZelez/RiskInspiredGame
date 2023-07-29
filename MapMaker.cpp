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

void MapMaker::handleEvent(const sf::RenderWindow &window, const sf::Event &event)
{
	switch(state)
	{
	case MapMakerState::none:
		handleButtons(event);
		break;
	case MapMakerState::territoryMode:
		territoryMaker.handleEvent(window, event);
		break;
	case MapMakerState::estateMode:
		estateMaker.handleEvent(event);
		break;
	}
	
}

void MapMaker::handleButtons(const sf::Event & event)
{
	for(Button &button : buttons)
	{
		button.handleEvent(event);
	}
}

void MapMaker::drawButtons(sf::RenderWindow & window)
{
	for(Button &button : buttons)
	{
		button.draw(window);
	}
}
