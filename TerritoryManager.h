#pragma once
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <vector>
#include <unordered_set>

class GameplaySettings;

class TerritoryManager
{
public:
	void drawPorts(sf::RenderWindow &window) const;

	// Load territories.
	void load(const GameplaySettings& gameplaySettings, std::string mapName);

	void draw(sf::RenderWindow &window) const;
	void setDrawMode(TerritoryDrawMode mode);

	bool positionClaimed(sf::Vector2f position) const;  // Returns true if any territory contains the world position.

	Territory* getTerritory(const sf::Vector2f &position);
	LandTerritory* getLandTerritory(sf::Vector2f position);  // Returns pointer to land territory at world position.
	NavalTerritory* getNavalTerritory(sf::Vector2f position);  // Returns pointer to nval territory at world position.

	std::vector<std::unique_ptr<LandTerritory>> &getLandTerritories();
	std::vector<std::unique_ptr<NavalTerritory>> &getNavalTerritories();
	const std::vector<const Territory*> getTerritories() const;

private:
	void calculateAdjacencies();
	void calculateDistances();

	void loadLandTerritory(const GameplaySettings& gameplaySettings, std::ifstream& file);
	void loadNavalTerritory(std::ifstream &file);

	NavalTerritory *getNavalTerritory(int id);

	std::vector<Territory*> territories;
	std::vector<std::unique_ptr<NavalTerritory>> navalTerritories;
	std::vector<std::unique_ptr<LandTerritory>> landTerritories;
};

Grid loadTerritoryGrid(std::ifstream& file);