#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>

enum class MapMode
{
	realm,
	barony,
	county,
	duchy,
	kingdom,
	empire,
	prosperity
};

class MapHandler
{
public:
	MapHandler(std::string name, const std::list<Player> &players);

	void draw(sf::RenderWindow &window) const;

	void changeMapMode(MapMode mode);

private:
	Map map;
	const std::list<Player> &players;

	bool displayMilitary = true;
	MapMode mode = MapMode::realm;

};


