#include "TerritoryManager.h"
#include "Territory.h"
#include "GameplaySettings.h"
#include "EditorGrid.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

void TerritoryManager::drawPorts(sf::RenderWindow & window) const
{
	for(const auto &territory : landTerritories)
	{
		territory.get()->drawPort(window);
	}
}

void TerritoryManager::load(const GameplaySettings& gameplaySettings, std::string mapName)
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
			loadLandTerritory(gameplaySettings, file);
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

void TerritoryManager::loadLandTerritory(const GameplaySettings& gameplaySettings, std::ifstream &file)
{
	Grid grid = loadTerritoryGrid(file);

	int id = loadTerritoryID(file);

	LandTerritoryFeatures features = loadLandTerritoryFeatures(&gameplaySettings, file);

	// Load naval territory associated with port if exists.
	NavalTerritory* portNavalTerritory = nullptr;
	if(file.peek() == '#')                             
	{
		int portNavalID = loadPortNavalID(file);
		portNavalTerritory = getNavalTerritory(portNavalID);
	}

	std::unique_ptr<LandTerritory> territory = 
	 	std::make_unique<LandTerritory>(id, grid, features, portNavalTerritory);
	
	territories.emplace_back(territory.get());
	landTerritories.emplace_back(std::move(territory));
}

void TerritoryManager::loadNavalTerritory(std::ifstream & file)
{
	Grid grid = loadTerritoryGrid(file);
	int id = loadTerritoryID(file);
	std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>(id, grid);
	territories.emplace_back(territory.get());
	navalTerritories.emplace_back(std::move(territory));
}

NavalTerritory* TerritoryManager::getNavalTerritory(int id)
{
	for(auto &territory : navalTerritories)
	{
		if(territory.get()->getId() == id)
		{
			return territory.get();
		}
	}
	return nullptr;
}

Grid loadTerritoryGrid(std::ifstream& file)
{
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;

	std::string line;
	int num1;
	int num2;

	std::getline(file, line);
	assert(line.compare(gridSaveLabel) == 0);
	while (std::getline(file, line) && line.size() > 0 && line[0] != '#')
	{
		std::istringstream iss(line);
		if (iss >> num1 >> num2)
		{
			positions.insert(sf::Vector2i(num1, num2));
		}
		else
		{
			// Error with file format.
			exit(1);
		}
	}

	// Determine default color from file.
	std::getline(file, line);
	std::istringstream iss(line);
	int r, g, b;
	iss >> r >> g >> b;
	sf::Color defaultColor(r, g, b);

	Grid grid(positions);
	// TEMP. GRID SHOULD STORE DEFAULT COLOR. PASS TO CONSTRUCTOR.
	grid.setInteriorColor(defaultColor);
	////////////

	return grid;
}
