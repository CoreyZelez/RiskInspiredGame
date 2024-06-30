#include "Map.h"
#include "GameplaySettings.h"
#include <fstream>
#include <iostream>

Map::Map(std::string name, const GameplaySettings *gameSettings)
	: name(name)
{
	// Attempts to load map from file. If not in files then map is empty.
	load(name, gameSettings);
}

void Map::load(std::string name, const GameplaySettings *gameSettings)
{
	territoryManager.load(name, gameSettings);
	estateManager.load(name, territoryManager.getLandTerritories(), territoryManager.getNavalTerritories());
}

TerritoryManager& Map::getTerritoryManager()
{
	return territoryManager;
}

const TerritoryManager & Map::getTerritoryManager() const
{
	return territoryManager;
}

EstateManager& Map::getEstateManager()
{
	return estateManager;
}

const EstateManager & Map::getEstateManager() const
{
	return estateManager;
}


