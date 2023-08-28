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

class MapView
{
public:
	MapView(const Map &map, const std::vector<std::unique_ptr<Player>> &players);

	void draw(sf::RenderWindow &window) const;

	void setMapMode(MapMode mode);
	void changeDisplayMilitary();

private:
	const Map &map;
	const std::vector<std::unique_ptr<Player>> &players;

	bool displayMilitary = true;
	MapMode mode = MapMode::realm;
};


