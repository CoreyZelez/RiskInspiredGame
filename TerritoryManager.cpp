#include "TerritoryManager.h"
#include "Territory.h"
#include "GameplaySettings.h"
#include <assert.h>
#include <iostream>
#include <fstream>

void TerritoryManager::drawPorts(sf::RenderWindow & window) const
{
	for(const auto &territory : landTerritories)
	{
		territory.get()->drawPort(window);
	}
}

void TerritoryManager::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");

	// Save naval territories before land territories since when loading land territories, require naval 
	// territories to already be loaded such that ports can be successfully loaded for the land territories.
	for(const auto& territory : navalTerritories)
	{
		territory.get()->saveToFile(file);
		file << std::endl; // Seperates territory save information.
	}

	// Save land territories.
	for(const auto& territory : landTerritories)
	{
		territory.get()->saveToFile(file);
		file << std::endl;  // Seperates territory save information.
	}
}

void TerritoryManager::load(std::string mapName, const GameplaySettings *gameplaySettings)
{
	std::ifstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");
	std::string line;

	while(std::getline(file, line))
	{
		if(line.size() == 0)  // Line is blank.
		{
			continue;
		}
		else if(line.compare(landSaveLabel) == 0)
		{
			loadLandTerritory(file, gameplaySettings);
		}
		else if(line.compare(navalSaveLabel) == 0)
		{
			loadNavalTerritory(file);
		}
	}

	calculateAdjacencies();
	calculateDistances();
}

void TerritoryManager::draw(sf::RenderWindow &window) const
{
	for(const Territory *territory : territories)
	{
		territory->draw(window);
	}
}

void TerritoryManager::setDrawMode(TerritoryDrawMode mode)
{
	for(Territory *territory : territories)
	{
		territory->setDrawMode(mode);
	}
}

bool TerritoryManager::positionClaimed(sf::Vector2f position) const
{
	// Checks land territories for position.
	for(auto &territory : landTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return true;
		}
	}

	// Checks naval territories for position.
	for(auto &territory : navalTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return true;
		}
	}

	return false;
}

Territory* TerritoryManager::getTerritory(const sf::Vector2f &position)
{
	for(auto &territory : territories)
	{
		if(territory->getGrid().containsPosition(position))
		{
			return territory;
		}
	}
	return nullptr;
}

LandTerritory* TerritoryManager::getLandTerritory(sf::Vector2f position)
{
	for(const auto &territory : landTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return territory.get();
		}
	}
	return nullptr;
}

NavalTerritory* TerritoryManager::getNavalTerritory(sf::Vector2f position)
{
	for(const auto &territory : navalTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return territory.get();
		}
	}
	return nullptr;
}

std::vector<std::unique_ptr<LandTerritory>>& TerritoryManager::getLandTerritories()
{
	return landTerritories;
}

std::vector<std::unique_ptr<NavalTerritory>>& TerritoryManager::getNavalTerritories()
{
	return navalTerritories;
}

const std::vector<const Territory*> TerritoryManager::getTerritories() const
{
	std::vector<const Territory*> territories;
	for(auto &territory : landTerritories)
	{
		territories.push_back(territory.get());
	}
	for(auto &territory : navalTerritories)
	{
		territories.push_back(territory.get());
	}
	return territories;
}

void TerritoryManager::calculateAdjacencies()
{
	for(auto &territory : territories)
	{
		territory->getDistanceMap().addAdjacencies(territories);
	}
}

void TerritoryManager::calculateDistances()
{
	for(auto &territory : territories)
	{
		territory->calculateDistances(territories);
	}
}

void TerritoryManager::loadLandTerritory(std::ifstream &file, const GameplaySettings *gameplaySettings)
{
	EditorGrid graphics = loadTerritoryGrid(file);

	int id = loadTerritoryID(file);

	LandTerritoryFeatures features = loadLandTerritoryFeatures(file, gameplaySettings);

	// Load naval territory associated with port if exists.
	NavalTerritory* portNavalTerritory = nullptr;
	if(file.peek() == '#')                             
	{
		int portNavalID = loadPortNavalID(file);
		portNavalTerritory = getNavalTerritory(portNavalID);
	}

	std::unique_ptr<LandTerritory> territory = 
	 	std::make_unique<LandTerritory>(id, graphics, features, portNavalTerritory);
	
	territories.emplace_back(territory.get());
	landTerritories.emplace_back(std::move(territory));
}

void TerritoryManager::loadNavalTerritory(std::ifstream & file)
{
	EditorGrid graphics = loadTerritoryGrid(file);
	int id = loadTerritoryID(file);
	std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>(id, graphics);
	territories.emplace_back(territory.get());
	navalTerritories.emplace_back(std::move(territory));
}

NavalTerritory* TerritoryManager::getNavalTerritory(int id)
{
	for(auto &territory : navalTerritories)
	{
		if(territory.get()->getID() == id)
		{
			return territory.get();
		}
	}
	return nullptr;
}
