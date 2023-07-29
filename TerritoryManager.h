#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>

class TerritoryManager
{
public:
	void draw(sf::RenderWindow &window);

	void removeEmptyTerritories();

	bool positionClaimed(sf::Vector2f position) const;  // Returns true if any territory contains the world position.

	LandTerritory* createLandTerritory();  // Creates empty land territory and returns handle.
	void removeLandTerritory(LandTerritory **territory);  // Removes NavalTerritory and nulls pointer.
	LandTerritory* getLandTerritory(sf::Vector2f position);  // Returns pointer to land territory at world position.

	NavalTerritory* createNavalTerritory();  // Creates empty naval territory and returns handle.
	void removeNavalTerritory(NavalTerritory **territory);  // Removes NavalTerritory and nulls pointer.
	NavalTerritory* getNavalTerritory(sf::Vector2f position);  // Returns pointer to nval territory at world position.

private:
	std::vector<std::unique_ptr<LandTerritory>> landTerritories;
	std::vector<std::unique_ptr<NavalTerritory>> navalTerritories;
};
