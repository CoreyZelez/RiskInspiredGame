#pragma once
#include "TerritoryManager.h"
#include "EstateManager.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "CoastalTerritory.h"
#include <SFML/Graphics.hpp>

class Map
{
public:
	void drawTerritoryMap(sf::RenderWindow &window);

	TerritoryManager &getTerritoryManager();
	EstateManager &getEstateManager();

private:
	TerritoryManager territoryManager;
	EstateManager estateManager;
};

