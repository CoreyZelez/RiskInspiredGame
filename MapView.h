#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>

enum class MapMode
{
	selectedRealm,  // Displays selected realm in yellow. All other realms displayed grey.
	realm,
	barony,
	county,
	duchy,
	kingdom,
	empire,
	prosperity
};

class Game;

class MapView
{
public:
	MapView(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>> &players);

	void draw(sf::RenderWindow &window) const;

	void setMapMode(MapMode mode);
	MapMode getMapMode() const;

private:
	const Game &game;
	const Map &map;
	const std::vector<std::unique_ptr<Player>> &players;

	MapMode mode = MapMode::realm;
};


