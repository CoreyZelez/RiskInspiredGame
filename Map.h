#pragma once
#include "TerritoryManager.h"
#include "EstateManager.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "CoastalTerritory.h"
#include <SFML/Graphics.hpp>
#include <string>

class Map
{
public:
	void drawTerritoryMap(sf::RenderWindow &window);

	void save(std::string name) const;
	void load(std::string name);

	TerritoryManager &getTerritoryManager();
	EstateManager &getEstateManager();

private:
	TerritoryManager territoryManager;
	EstateManager estateManager;
};

