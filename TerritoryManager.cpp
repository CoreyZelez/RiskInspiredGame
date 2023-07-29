#include "TerritoryManager.h"
#include <assert.h>
#include <iostream>

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
