#include "MapMaker.h"
#include "ChangeInstance.h"
#include "InputUtility.h"
#include "GameplaySettings.h"
#include <assert.h>
#include <iostream>
#include <filesystem>

MapMaker::MapMaker(std::string mapName)
	: mapName(mapName)
{
	load(mapName);
}

void MapMaker::save()
{
	saveAs(mapName);
}

void MapMaker::saveAs(std::string mapName)
{
	namespace fs = std::filesystem;

	const std::string folderPath = "res/maps/" + mapName;

	// Create the folder
	if (!fs::exists(folderPath))
	{
		if (!fs::create_directory(folderPath))
		{
			std::cerr << "Failed to create the folder." << std::endl;
			exit(1);
		}
	}

	territoryMaker.save(mapName);
	estateMaker.save(mapName);
}

void MapMaker::load(std::string mapName)
{
	territoryMaker.load(mapName);
	estateMaker.load(mapName, territoryMaker.getTerritories());
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
			estateMaker.reconcileBaronies(territoryMaker.getTerritories());
			estateMaker.reconcileMaridoms(territoryMaker.getTerritories());
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

