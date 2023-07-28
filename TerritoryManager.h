#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>

class TerritoryManager
{
public:
	void draw(sf::RenderWindow &window);

	void addLandTerritory(std::unique_ptr<LandTerritory> territory);
	void removeLandTerritory(LandTerritory **territory);  // Removes NavalTerritory and nulls pointer.
	LandTerritory* getLandTerritory(sf::Vector2f worldPosition);

	void addNavalTerritory(std::unique_ptr<NavalTerritory> territory);
	void removeNavalTerritory(NavalTerritory **territory);  // Removes NavalTerritory and nulls pointer.
	NavalTerritory* getNavalTerritory(sf::Vector2f worldPosition);

private:
	std::vector<Territory*> territories;
	std::vector<std::unique_ptr<LandTerritory>> landTerritories;
	std::vector<std::unique_ptr<NavalTerritory>> navalTerritories;
	// should we have vector of Territory* and CoastalTerritory*?
};

