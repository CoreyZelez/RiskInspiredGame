#pragma once
#include "TerritoryManager.h"
#include "EstateManager.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <SFML/Graphics.hpp>
#include <string>

class Map
{
public:
	Map(std::string name);

	void drawTerritoryMap(sf::RenderWindow &window);

	void save();
	void saveAs(std::string name);  // Save map under different name.

	TerritoryManager &getTerritoryManager();
	EstateManager &getEstateManager();

private:
	void load(std::string name);

	std::string name;
	TerritoryManager territoryManager;
	EstateManager estateManager;
};
