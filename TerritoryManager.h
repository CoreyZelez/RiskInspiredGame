#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>

class TerritoryManager
{
public:
	void draw(sf::RenderWindow &window) const;

	void save(std::string mapName) const;
	void load(std::string mapName);

	void removeEmptyTerritories();

	bool positionClaimed(sf::Vector2f position) const;  // Returns true if any territory contains the world position.

	void convertLandsToCoastal();  // Converts land territories bordering naval territories into coastal territories.

	Territory* getTerritory(const sf::Vector2f &position);

	LandTerritory* createLandTerritory();  // Creates empty land territory and returns handle.
	void removeLandTerritory(LandTerritory *territory);  // Removes NavalTerritory and nulls pointer.
	LandTerritory* getLandTerritory(sf::Vector2f position);  // Returns pointer to land territory at world position.

	NavalTerritory* createNavalTerritory();  // Creates empty naval territory and returns handle.
	void removeNavalTerritory(NavalTerritory *territory);  // Removes NavalTerritory and nulls pointer.
	NavalTerritory* getNavalTerritory(sf::Vector2f position);  // Returns pointer to nval territory at world position.

	std::vector<std::unique_ptr<LandTerritory>> &getLandTerritories();

private:
	void calculateAdjacencies();
	void calculateDistances();

	void loadLandTerritory(std::ifstream &file);
	void loadNavalTerritory(std::ifstream &file);
	void removeTerritory(Territory *territory);

	std::vector<Territory*> territories;
	std::vector<std::unique_ptr<NavalTerritory>> navalTerritories;
	std::vector<std::unique_ptr<LandTerritory>> landTerritories;



	int nextID = 0;  // Next ID to be assigned to newly created territory.
};


