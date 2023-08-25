#pragma once
#include <SFML/Graphics.hpp>
#include "MapView.h"
#include "GameUI.h"

class Estate;

class GameView
{
public:
	GameView(const Map &map, const std::list<Player> &players);
	void draw(sf::RenderWindow &window) const;

private:
	GameUI gameUI;
	MapView mapView;
};

