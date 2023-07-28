#include "TerritoryManager.h"
#include <assert.h>

void TerritoryManager::draw(sf::RenderWindow &window)
{
	// Draws land territories (including coastal territories).
	for(const auto &territory : landTerritories)
	{
		territory.get()->draw(window);
	}

	// Draws naval territories.
	for(const auto &territory : landTerritories)
	{
		territory.get()->draw(window);
	}
}

void TerritoryManager::addLandTerritory(std::unique_ptr<LandTerritory> territory)
{
	landTerritories.emplace_back(std::move(territory));
}

void TerritoryManager::addNavalTerritory(std::unique_ptr<NavalTerritory> territory)
{
	navalTerritories.emplace_back(std::move(territory));
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

LandTerritory* TerritoryManager::getLandTerritory(sf::Vector2f worldPosition)
{
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

NavalTerritory* TerritoryManager::getNavalTerritory(sf::Vector2f worldPosition)
{
	return nullptr;
}
