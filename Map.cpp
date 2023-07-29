#include "Map.h"

void Map::drawTerritoryMap(sf::RenderWindow &window)
{
	territoryManager.draw(window);
}

TerritoryManager& Map::getTerritoryManager()
{
	return territoryManager;
}

EstateManager& Map::getEstateManager()
{
	return estateManager;
}
