#pragma once
#include "Map.h"
#include "TerritoryMaker.h"
#include "EstateMaker.h"
#include "CommandButton.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class GameplaySettings;

class MapMaker
{
	enum class MapMakerState
	{
		TerritoryMode,
		EstateMode
	};

public:
	MapMaker(std::string mapName);

	void save();
	void saveAs(std::string mapName);

	void draw(sf::RenderWindow &window);

	void handleInput(const sf::RenderWindow &window, sf::View &view);

private:
	MapMakerState state = MapMakerState::TerritoryMode;
	TerritoryMaker territoryMaker;
	EstateMaker estateMaker;
	const std::string mapName;

	void load(std::string mapName);
};

