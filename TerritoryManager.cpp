#include "TerritoryManager.h"
#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

void TerritoryManager::draw(sf::RenderWindow &window)
{
	// Draws land territories (including coastal territories).
	for(const auto &territory : landTerritories)
	{
		territory.get()->draw(window);
	}

	// Draws naval territories.
	for(const auto &territory : navalTerritories)
	{
		territory.get()->draw(window);
	}
}

void TerritoryManager::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");

	for(auto& territory : landTerritories)
	{
		territory.get()->saveToFile(file);
	}

	for(auto& territory : navalTerritories)
	{
		territory.get()->saveToFile(file);
	}
}

void TerritoryManager::load(std::string mapName)
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
			Grid graphics = loadTerritoryGrid(file);
			int id = loadTerritoryID(file);
			std::shared_ptr<LandTerritory> territory = std::make_shared<LandTerritory>(id, graphics);
			landTerritories.push_back(std::move(territory));

			// Ensure next id greater than all other territory ids.
			nextID = std::max(nextID, id + 1);
		}
		else if(line.compare(navalSaveLabel) == 0)
		{
			Grid graphics = loadTerritoryGrid(file);
			int id = loadTerritoryID(file);
			std::shared_ptr<NavalTerritory> territory = std::make_shared<NavalTerritory>(id, graphics);
			navalTerritories.push_back(std::move(territory));

			// Ensure next id greater than all other territory ids.
			nextID = std::max(nextID, id + 1);
		}
	}
}

void TerritoryManager::removeEmptyTerritories()
{
	// Remove empty land territories.
	auto iterL = landTerritories.begin();
	while(iterL != landTerritories.end())
	{
		if((*iterL)->isEmpty())
		{
			iterL = landTerritories.erase(iterL);
		}
		else
		{
			++iterL;
		}
	}

	// Remove empty naval territories.
	auto iterN = navalTerritories.begin();
	while(iterN != navalTerritories.end())
	{
		if((*iterN)->isEmpty())
		{
			iterN = navalTerritories.erase(iterN);
		}
		else
		{
			++iterN;
		}
	}
}

bool TerritoryManager::positionClaimed(sf::Vector2f position) const
{
	// Checks land territories for position.
	for(const auto &territory : landTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return true;
		}
	}

	// Checks naval territories for position.
	for(const auto &territory : navalTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return true;
		}
	}

	return false;
}

void TerritoryManager::convertLandsToCoastal()
{
}

std::shared_ptr<LandTerritory> TerritoryManager::createLandTerritory()
{
	std::shared_ptr<LandTerritory> territory = std::make_shared<LandTerritory>(nextID++);
	landTerritories.emplace_back(territory);
	return territory;
}

std::shared_ptr<NavalTerritory> TerritoryManager::createNavalTerritory()
{
	std::shared_ptr<NavalTerritory> territory = std::make_shared<NavalTerritory>(nextID++);
	navalTerritories.emplace_back(territory);
	return territory;
}

void TerritoryManager::removeLandTerritory(std::shared_ptr<LandTerritory> &territory)
{
	auto iter = landTerritories.begin();
	while(iter != landTerritories.end())
	{
		if(iter->get() == territory.get())
		{
			landTerritories.erase(iter);
			territory = nullptr;  // Invalidate future usage of pointer.
			return;
		}
		++iter;
	}
	assert(false);
}

std::shared_ptr<LandTerritory> TerritoryManager::getLandTerritory(sf::Vector2f position)
{
	for(const auto &territory : landTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return territory;
		}
	}
	return nullptr;
}

void TerritoryManager::removeNavalTerritory(std::shared_ptr<NavalTerritory> &territory)
{
	auto iter = navalTerritories.begin();
	while(iter != navalTerritories.end())
	{
		if(iter->get() == territory.get())
		{
			navalTerritories.erase(iter);
			territory = nullptr;  // Invalidate future usage of pointer.
			return;
		}
		++iter;
	}
	assert(false);
}

std::shared_ptr<NavalTerritory> TerritoryManager::getNavalTerritory(sf::Vector2f position)
{
	for(const auto &territory : navalTerritories)
	{
		if(territory.get()->getGrid().containsPosition(position))
		{
			return territory;
		}
	}
	return nullptr;
}

const std::vector<std::shared_ptr<LandTerritory>>& TerritoryManager::getLandTerritories() const
{
	return landTerritories;
}
