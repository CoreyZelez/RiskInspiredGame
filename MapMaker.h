#pragma once
#include "Map.h"
#include "TerritoryMaker.h"
#include "EstateMaker.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

enum class MapMakerState
{
	none,
	territoryMode,
	estateMode
};

class MapMaker
{
public:
	MapMaker();

	void draw(sf::RenderWindow &window);

	void handleEvent(const sf::RenderWindow &window, const sf::Event &event);

private:
	void handleButtons(const sf::Event &event);
	void drawButtons(sf::RenderWindow &window);

	Map map;
	MapMakerState state = MapMakerState::none;

	TerritoryMaker territoryMaker;
	EstateMaker estateMaker;

	std::vector<Button> buttons;
};

