#pragma once
#include "Grid.h"
#include "Terrain.h"
#include "Culture.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <map>

class Territory;
class TerritoryManager;
class LandTerritory;
class NavalTerritory;

enum class TerritoryMakerState
{
	none,
	editTerritoryGrid,  // Adjust shape of territory by adding or removing grid squares.
	createPort,  // Create port by selecting a land naval territory pair.
	editCulture,
	editTerrain,
	editCoreProsperity
};

class TerritoryMaker
{
public:
	TerritoryMaker(TerritoryManager &territoryManager);

	void draw(sf::RenderWindow &window) const;

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	TerritoryManager &territoryManager;

	TerritoryMakerState state = TerritoryMakerState::none;

	Territory *selectedTerritory = nullptr;  // Territory currently being modified.

	std::pair<LandTerritory*, NavalTerritory*> portTerritories = { nullptr, nullptr };  // Territories chosen for creation of port.

	int brushSize = 3;  // Width of square positions when adding or removing territory squares.

	std::unordered_set<sf::Vector2i, Vector2iHash> claimedPositions;  // Grid positions claimed by any territory 
	sf::VertexArray fixedTerritoryVertices;	 // Vertex array of all territories excluding territory currently being modified.

	Culture selectedCulture;  

	Terrain selectedTerrain;  

	sf::Clock inputClock;  	// Clock tracking time since last time input handled.

	void changeState(TerritoryMakerState state);

	void handleInputForView(sf::View &view) const;
	void handleInputForStateChange();
	void handleInputForPortCreation(const sf::RenderWindow &window);
	void handleInputForTerritorySelection(const sf::RenderWindow &window);
	void handleInputForTerritoryCreation();
	void handleInputForTerritoryGridEdits(const sf::RenderWindow &window);
	void handleInputForTerrainChange(const sf::RenderWindow &window);
	void handleInputForCultureChange(const sf::RenderWindow &window);

	void handleTerrainKeyPress(sf::Keyboard::Key key, int terrainNum);
	void handleCultureKeyPress(sf::Keyboard::Key key, int cultureNum);

	void initClaimedPositions();
	void updateFixedTerritoriesVertices();
	void progressBrushSize();
	std::vector<sf::Vector2i> determineBrushGridPositions(sf::Vector2i centerPosition)const ;
	void addPosition(const sf::RenderWindow &window);
	void removePosition(const sf::RenderWindow &window);
};

