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
	TerritoryMaker(TerritoryManager &map);

	void draw(sf::RenderWindow &window);

	void handleEvent(const sf::RenderWindow &window, const sf::Event &event);

private:
	TerritoryManager &territoryManager;
	Territory *territory = nullptr;  // Territory currently being modified.
	TerritoryMakerState state = TerritoryMakerState::none;
};

