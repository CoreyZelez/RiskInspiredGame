#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>

class TerritoryManager
{
public:
	void addLandTerritory(LandTerritory territory);
	void removeLandTerritory(LandTerritory *territory);
	LandTerritory* getLandTerritory(sf::Vector2f worldPosition);

	void addNavalTerritory(NavalTerritory territory);
	void removeNavalTerritory(NavalTerritory *territory);
	NavalTerritory* getNavalTerritory(sf::Vector2f worldPosition);

private:
	std::vector<LandTerritory> landTerritories;
	std::vector<NavalTerritory> navalTerritories;
	// should we have vector of Territory* and CoastalTerritory*?
};

