#pragma once
#include "Map.h"
#include <memory>
#include <SFML/Graphics.hpp>

enum MapMakerState
{

};

class MapMaker
{
public:
	void draw(sf::RenderWindow &window);
	void handleInput();

private:
	void assertInvariants();

	Map map;

	std::unique_ptr<LandTerritory> currLand;
	std::unique_ptr<NavalTerritory> currNaval;

	MapMakerState state;
};

