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
	MapMaker(std::string mapName);

	void save();
	void saveAs(std::string mapName);
	void load(std::string mapName);

	void draw(sf::RenderWindow &window);

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	void handleButtonInput(const sf::RenderWindow &window);
	void drawButtons(sf::RenderWindow &window);

	Map map;
	MapMakerState state = MapMakerState::none;

	TerritoryMaker territoryMaker;
	EstateMaker estateMaker;

	std::vector<std::unique_ptr<Button>> buttons;
};

