#pragma once
#include <SFML/Graphics.hpp>

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
	Territory *territory = nullptr; 
	// Territories chosen for creation of port.
	std::pair<LandTerritory*, NavalTerritory*> portTerritories = { nullptr, nullptr };
	// Territory maker state representing how user input should be interpreted.
	TerritoryMakerState state = TerritoryMakerState::none;
	// Clock tracking time since last time input handled.
	sf::Clock inputClock;  
};

