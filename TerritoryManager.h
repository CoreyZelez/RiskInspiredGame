#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>
#include <unordered_set>

class TerritoryManager
{
public:
	void drawPorts(sf::RenderWindow &window) const;

	void save(std::string mapName) const;
	void load(std::string mapName);

	void draw(sf::RenderWindow &window) const;
	void setDrawMode(TerritoryDrawMode mode);

	void removeEmptyTerritories();

	bool positionClaimed(sf::Vector2f position) const;  // Returns true if any territory contains the world position.

	Territory* getTerritory(const sf::Vector2f &position);

	LandTerritory* createLandTerritory();  // Creates empty land territory and returns handle.
	void removeLandTerritory(LandTerritory *territory);  // Removes NavalTerritory and nulls pointer.
	LandTerritory* getLandTerritory(sf::Vector2f position);  // Returns pointer to land territory at world position.

	NavalTerritory* createNavalTerritory();  // Creates empty naval territory and returns handle.
	void removeNavalTerritory(NavalTerritory *territory);  // Removes NavalTerritory and nulls pointer.
	NavalTerritory* getNavalTerritory(sf::Vector2f position);  // Returns pointer to nval territory at world position.

	std::vector<std::unique_ptr<LandTerritory>> &getLandTerritories();
	std::vector<std::unique_ptr<NavalTerritory>> &getNavalTerritories();
	const std::vector<const Territory*> getTerritories() const;

private:
	void calculateAdjacencies();
	void calculateDistances();

	void loadLandTerritory(std::ifstream &file);
	void loadNavalTerritory(std::ifstream &file);
	void removeTerritory(Territory *territory);
	NavalTerritory *getNavalTerritory(int id);

	std::vector<Territory*> territories;
	std::vector<std::unique_ptr<NavalTerritory>> navalTerritories;
	std::vector<std::unique_ptr<LandTerritory>> landTerritories;

	int nextID = 0;  // Next ID to be assigned to newly created territory.
};


