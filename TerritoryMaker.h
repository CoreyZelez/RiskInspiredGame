#pragma once
#include "EditorGrid.h"
#include "Terrain.h"
#include "Culture.h"
#include "LandTerritoryFeatureEditor.h"
#include "LandTerritoryFeatureEditor.cpp"
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
	editProsperities
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
	std::unordered_set<sf::Vector2i, Vector2iHash> claimedPositions;  // EditorGrid positions claimed by any territory 
	sf::VertexArray fixedTerritoryVertices;	 // Vertex array of all territories excluding territory currently being modified.
	sf::Clock inputClock;  	// Clock tracking time since last time input handled.

	LandTerritoryFeatureEditor<Terrain> terrainEditor;
	LandTerritoryFeatureEditor<Culture> cultureEditor;
	LandTerritoryFeatureEditor<int> prosperityEditor;

	void changeState(TerritoryMakerState state);
	void handleInputForView(sf::View &view) const;
	void handleInputForStateChange();

	void handleInputForPortCreation(const sf::RenderWindow &window);
	void handleInputForTerritorySelection(const sf::RenderWindow &window);
	void handleInputForTerritoryCreation();
	void handleInputForTerritoryGridEdits(const sf::RenderWindow &window);

	void initClaimedPositions();
	void updateFixedTerritoriesVertices();
	void progressBrushSize();
	std::vector<sf::Vector2i> determineBrushGridPositions(sf::Vector2i centerPosition)const ;
	void addPosition(const sf::RenderWindow &window);
	void removePosition(const sf::RenderWindow &window);
};

