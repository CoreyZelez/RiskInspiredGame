#include "TerritoryManager.h"
#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

void TerritoryManager::draw(sf::RenderWindow &window) const
{
	for(const Territory *territory : territories)
	{
		territory->draw(window);
	}
}

void TerritoryManager::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");

	for(const auto& territory : landTerritories)
	{
		territory.get()->saveToFile(file);
	}

	for(const auto& territory : navalTerritories)
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
			loadLandTerritory(file);
		}
		else if(line.compare(navalSaveLabel) == 0)
		{
			loadNavalTerritory(file);
		}
	}

	removeEmptyTerritories();
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

void TerritoryManager::convertLandsToCoastal()
{
}

LandTerritory* TerritoryManager::createLandTerritory()
{
	std::unique_ptr<LandTerritory> territory = std::make_unique<LandTerritory>(nextID++);
	LandTerritory *t = territory.get();
	landTerritories.emplace_back(std::move(territory));
	territories.emplace_back(t);
	return t;
}

NavalTerritory* TerritoryManager::createNavalTerritory()
{
	std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>(nextID++);
	NavalTerritory *t = territory.get();
	navalTerritories.emplace_back(std::move(territory));
	territories.emplace_back(t);
	return t;
}

void TerritoryManager::removeLandTerritory(LandTerritory *territory)
{
	removeTerritory(territory);

	auto iter = landTerritories.begin();
	while(iter != landTerritories.end())
	{
		if(iter->get() == territory)
		{
			landTerritories.erase(iter);
			territory = nullptr;  // Invalidate future usage of pointer.
			return;
		}
		++iter;
	}
	assert(false);
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

void TerritoryManager::removeNavalTerritory(NavalTerritory *territory)
{
	removeTerritory(territory);

	auto iter = navalTerritories.begin();
	while(iter != navalTerritories.end())
	{
		if(iter->get() == territory)
		{
			navalTerritories.erase(iter);
			territory = nullptr;  // Invalidate future usage of pointer.
			return;
		}
		++iter;
	}
	assert(false);
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

void TerritoryManager::loadLandTerritory(std::ifstream & file)
{
	Grid graphics = loadTerritoryGrid(file);
	int id = loadTerritoryID(file);
	std::unique_ptr<LandTerritory> territory = std::make_unique<LandTerritory>(id, graphics);
	territories.emplace_back(territory.get());
	landTerritories.emplace_back(std::move(territory));


	nextID = std::max(nextID, id + 1);  // Ensure next id greater than all other territory ids.
}

void TerritoryManager::loadNavalTerritory(std::ifstream & file)
{
	Grid graphics = loadTerritoryGrid(file);
	int id = loadTerritoryID(file);
	std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>(id, graphics);
	territories.emplace_back(territory.get());
	navalTerritories.emplace_back(std::move(territory));

	nextID = std::max(nextID, id + 1);  // Ensure next id greater than all other territory ids.
}

void TerritoryManager::removeTerritory(Territory *territory)
{
	for(auto iter = territories.begin(); iter != territories.end(); ++iter)
	{
		if(*iter == territory)
		{
			territories.erase(iter);
			return;
		}
	}
	assert(false);  // Should only ever be called on a territory that exists in territories.
}
