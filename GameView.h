#pragma once
#include <SFML/Graphics.hpp>
#include "MapView.h"
#include "GameUI.h"

class Estate;

class GameView
{
public:
	GameView(const Map &map, const std::vector<std::unique_ptr<Player>>& players);
	void draw(sf::RenderWindow &window) const;

	void setMapMode(MapMode mapMode);

private:
	GameUI gameUI;  
	MapView mapView;  
};

