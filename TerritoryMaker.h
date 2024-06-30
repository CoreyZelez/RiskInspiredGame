#pragma once
#include "EditorTerritory.h"
#include "Territory.h"
#include "Terrain.h"
#include "Culture.h"
#include "LandTerritoryFeatureEditor.h"
#include "LandTerritoryFeatureEditor.cpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <map>
#include <deque>

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
	TerritoryMaker();

	void load(std::string mapName);

	void save(std::string mapName) const;

	void draw(sf::RenderWindow &window) const;

	void handleInput(const sf::RenderWindow &window, sf::View &view);

	std::deque<EditorTerritory>& getTerritories();
	EditorTerritory* getTerritory(sf::Vector2f position);

private:
	TerritoryMakerState state = TerritoryMakerState::none;

	std::deque<EditorTerritory> territories;
	EditorTerritory *selectedTerritory = nullptr;  // Territory currently being modified.

	int nextID = 0;  // Next ID to be assigned to newly created territory.
	std::unordered_set<sf::Vector2i, Vector2iHash> claimedPositions;  // EditorGrid positions claimed by any territory 
	sf::VertexArray fixedTerritoryVertices;	 // Vertex array of all territories excluding territory currently being modified.
	int brushSize = 3;  // Width of square positions when adding or removing territory squares.
	sf::Clock inputClock;  	// Clock tracking time since last time input handled.

	LandTerritoryFeatureEditor<Terrain> terrainEditor;
	LandTerritoryFeatureEditor<Culture> cultureEditor;
	LandTerritoryFeatureEditor<int> prosperityEditor;

	void loadLandTerritory(std::ifstream& file);
	void loadNavalTerritory(std::ifstream& file);

	void changeState(TerritoryMakerState state);
	void handleInputForView(sf::View &view) const;
	void handleInputForStateChange();

	void handleInputForPortCreation(const sf::RenderWindow &window);
	void handleInputForTerritorySelection(const sf::RenderWindow &window);
	void handleInputForTerritoryCreation();
	void handleInputForTerritoryGridEdits(const sf::RenderWindow &window);

	void drawPorts(sf::RenderWindow& window) const;
	void setDrawMode(TerritoryDrawMode mode);

	void initClaimedPositions();
	void updateFixedTerritoriesVertices();
	void progressBrushSize();
	std::vector<sf::Vector2i> determineBrushGridPositions(sf::Vector2i centerPosition)const ;
	void addPosition(const sf::RenderWindow &window);
	void removePosition(const sf::RenderWindow &window);

	void removeEmptyTerritories();
	void removeTerritory(EditorTerritory& territory);

	// Creates empty territory of specific type and returns handle.
	EditorTerritory& createTerritory(TerritoryType type);

	// Creates port between selected land territory and parameter naval territory if adjacent.
	void createPort(EditorTerritory& navalTerritory);

	EditorTerritory* getTerritory(TerritoryType type, sf::Vector2f position);
	EditorTerritory* getTerritory(int id);

};

