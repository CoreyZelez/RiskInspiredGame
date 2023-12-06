#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Grid.h"

class Territory;
class TerritoryManager;
class LandTerritory;
class NavalTerritory;

enum class TerritoryMakerState
{
	none,
	editTerritory,  // Adjust shape of territory by adding or removing grid squares.
	createPort  // Create port by selecting a land naval territory pair.
};

class TerritoryMaker
{
public:
	TerritoryMaker(TerritoryManager &territoryManager);

	void draw(sf::RenderWindow &window) const;

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	void handleInputForView(sf::View &view) const;

	TerritoryManager &territoryManager;

	// Territory currently being modified.
	Territory *selectedTerritory = nullptr; 
	// Territories chosen for creation of port.
	std::pair<LandTerritory*, NavalTerritory*> portTerritories = { nullptr, nullptr };
	// Territory maker state representing how user input should be interpreted.
	TerritoryMakerState state = TerritoryMakerState::none;
	// Width of square positions when adding or removing territory squares.
	int brushSize = 3;
	// Grid positions claimed by any territory 
	std::unordered_set<sf::Vector2i, Vector2iHash> claimedPositions;
	// Clock tracking time since last time input handled.
	sf::Clock inputClock;  

	// Vertex array of all territories excluding territory currently being modified.
	sf::VertexArray fixedTerritoryVertices;

	void initClaimedPositions();
	void updateFixedTerritoriesVertices();
	void progressBrushSize();
	std::vector<sf::Vector2i> determineBrushGridPositions(sf::Vector2i centerPosition)const ;
	void addPosition(const sf::RenderWindow &window);
	void removePosition(const sf::RenderWindow &window);
};

