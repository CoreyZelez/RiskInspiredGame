#include "TerritoryManager.h"
#include <assert.h>

void TerritoryManager::addLandTerritory(LandTerritory territory)
{
	landTerritories.push_back(territory);
}

void TerritoryManager::addNavalTerritory(NavalTerritory territory)
{
	navalTerritories.push_back(territory);
}

void TerritoryManager::removeLandTerritory(LandTerritory *territory)
{
	auto iter = landTerritories.begin();
	while(iter != landTerritories.end())
	{
		if(&(*iter) == territory)
		{
			landTerritories.erase(iter);
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

void TerritoryManager::removeNavalTerritory(NavalTerritory *territory)
{
	auto iter = navalTerritories.begin();
	while(iter != navalTerritories.end())
	{
		if(&(*iter) == territory)
		{
			navalTerritories.erase(iter);
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
