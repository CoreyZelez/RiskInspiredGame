#include "MapMaker.h"
#include "ChangeInstance.h"
#include "ReconcileLandedEstates.h"
#include "InputUtility.h"
#include "GameplaySettings.h"
#include <assert.h>
#include <iostream>

MapMaker::MapMaker(std::string mapName)
	: map(mapName), territoryMaker(map.getTerritoryManager()), estateMaker(map.getEstateManager())
{
}

MapMaker::~MapMaker()
{
	save();
}

void MapMaker::save()
{
	map.save();
}

void MapMaker::saveAs(std::string mapName)
{
	map.saveAs(mapName);
}

void MapMaker::load(std::string mapName, const GameplaySettings &gameplaySettings)
{
	map = Map("mapName");
}

void MapMaker::draw(sf::RenderWindow &window)
{
	switch(state)
	{
	case MapMakerState::TerritoryMode:
		territoryMaker.draw(window);
		break;
	case MapMakerState::EstateMode:
		estateMaker.draw(window);
		break;
	}
}

void MapMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Change map maker mode.
	if(inputUtility.getKeyPressed(sf::Keyboard::Tab))
	{
		switch(state)
		{
		case MapMakerState::TerritoryMode:
			// Ensure all estates are reconciled with created and deleted territories.
			map.getEstateManager().reconcileBaronies(map.getTerritoryManager().getLandTerritories());
			map.getEstateManager().reconcileMaridoms(map.getTerritoryManager().getNavalTerritories());
			state = MapMakerState::EstateMode;
			break;
		case MapMakerState::EstateMode:
			state = MapMakerState::TerritoryMode;
			break;
		}

	}

	switch(state)
	{
	case MapMakerState::TerritoryMode:
		territoryMaker.handleInput(window, view);
		break;
	case MapMakerState::EstateMode:
		estateMaker.handleInput(window, view);
		break;
	}
}

