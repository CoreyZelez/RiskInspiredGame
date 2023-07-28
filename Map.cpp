#include "Map.h"

void Map::drawTerritoryMap(sf::RenderWindow &window)
{
	territoryManager.draw(window);
}

void Map::addLandTerritory(std::unique_ptr<LandTerritory> territory)
{
	territoryManager.addLandTerritory(std::move(territory));
}

void Map::removeLandTerritory(LandTerritory** territory)
{
	territoryManager.removeLandTerritory(territory);
}

LandTerritory* Map::getLandTerritory(sf::Vector2f worldPosition)
{
	return territoryManager.getLandTerritory(worldPosition);
}

void Map::addNavalTerritory(std::unique_ptr<NavalTerritory> territory)
{
	territoryManager.addNavalTerritory(std::move(territory));
}

void Map::removeNavalTerritory(NavalTerritory** territory)
{
	territoryManager.removeNavalTerritory(territory);
}

NavalTerritory* Map::getNavalTerritory(sf::Vector2f worldPosition)
{
	return territoryManager.getNavalTerritory(worldPosition);
}
