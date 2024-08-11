#include "Map.h"
#include "GameplaySettings.h"
#include <fstream>
#include <iostream>

Map::Map(const GameplaySettings& gameSettings, std::string name)
	: name(name), estateManager(gameSettings)
{
	// Attempts to load map from file. If not in files then map is empty.
	load(gameSettings, name);
}

void Map::load(const GameplaySettings& gameSettings, std::string name)
{
	territoryManager.load(gameSettings, name);
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


