#pragma once
#include <SFML/Graphics.hpp>

class Territory;
class TerritoryManager;

enum class TerritoryMakerState
{
	none,
	editTerritory
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
	Territory *territory = nullptr;  // Territory currently being modified.
	TerritoryMakerState state = TerritoryMakerState::none;
	sf::Clock inputClock;  // Clock tracking time since last time input handled.
};

