#include "TerritoryManager.h"
#include "Territory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

void TerritoryManager::draw(sf::RenderWindow &window)
{
	int cnt = 0;

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
			TerritoryGraphics graphics = loadTerritoryGraphics(file);
			std::unique_ptr<LandTerritory> territory = std::make_unique<LandTerritory>(graphics);
			landTerritories.push_back(std::move(territory));
		}
		else if(line.compare(navalSaveLabel) == 0)
		{
			TerritoryGraphics graphics = loadTerritoryGraphics(file);
			std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>(graphics);
			navalTerritories.push_back(std::move(territory));
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
		if(territory.get()->containsPosition(position))
		{
			return true;
		}
	}

	// Checks naval territories for position.
	for(const auto &territory : navalTerritories)
	{
		if(territory.get()->containsPosition(position))
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
	std::unique_ptr<LandTerritory> territory = std::make_unique<LandTerritory>();
	landTerritories.emplace_back(std::move(territory));
	return landTerritories.back().get();
}

NavalTerritory* TerritoryManager::createNavalTerritory()
{
	std::unique_ptr<NavalTerritory> territory = std::make_unique<NavalTerritory>();
	navalTerritories.emplace_back(std::move(territory));
	return navalTerritories.back().get();
}

void TerritoryManager::removeLandTerritory(LandTerritory **territory)
{
	auto iter = landTerritories.begin();
	while(iter != landTerritories.end())
	{
		if(iter->get() == *territory)
		{
			landTerritories.erase(iter);
			*territory = nullptr;  // Invalidate future usage of pointer.
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
		if(territory.get()->containsPosition(position))
		{
			return territory.get();
		}
	}
	return nullptr;
}

void TerritoryManager::removeNavalTerritory(NavalTerritory **territory)
{
	auto iter = navalTerritories.begin();
	while(iter != navalTerritories.end())
	{
		if(iter->get() == *territory)
		{
			navalTerritories.erase(iter);
			*territory = nullptr;  // Invalidate future usage of pointer.
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
		if(territory.get()->containsPosition(position))
		{
			return territory.get();
		}
	}
	return nullptr;
}
