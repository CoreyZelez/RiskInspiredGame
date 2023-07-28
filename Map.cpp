#include "Map.h"

void Map::addLandTerritory(LandTerritory territory)
{
	territoryManager.addLandTerritory(territory);
}

void Map::removeLandTerritory(LandTerritory *territory)
{
	territoryManager.removeLandTerritory(territory);
}

LandTerritory* Map::getLandTerritory(sf::Vector2f worldPosition)
{
	return territoryManager.getLandTerritory(worldPosition);
}

void Map::addNavalTerritory(NavalTerritory territory)
{
	territoryManager.addNavalTerritory(territory);
}

void Map::removeNavalTerritory(NavalTerritory* territory)
{
	territoryManager.removeNavalTerritory(territory);
}

NavalTerritory* Map::getNavalTerritory(sf::Vector2f worldPosition)
{
	return territoryManager.getNavalTerritory(worldPosition);
}
