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
	void addLandTerritory(LandTerritory territory);
	void removeLandTerritory(LandTerritory* territory);
	LandTerritory* getLandTerritory(sf::Vector2f worldPosition);

	void addNavalTerritory(NavalTerritory territory);
	void removeNavalTerritory(NavalTerritory* territory);
	NavalTerritory* getNavalTerritory(sf::Vector2f worldPosition);

private:
	TerritoryManager territoryManager;
	EstateManager estateManager;
};

