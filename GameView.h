#pragma once
#include <SFML/Graphics.hpp>
#include "MapView.h"
#include "GameUI.h"

class Estate;

class GameView
{
public:
	GameView(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>>& players);
	void draw(sf::RenderWindow &window) const;

	void setLeftPanel(const IHasUI &hasUI);
	void setRightPanel(const IHasUI &hasUI);
	void setBottomPanel(const IHasUI &hasUI);

	void setMapMode(MapMode mapMode);
	MapMode getMapMode() const;

private:

	void drawMilitaries(sf::RenderWindow &window) const;

	const Game &game;
	const std::vector<std::unique_ptr<Player>> &players;
	GameUI gameUI;  
	MapView mapView;  
};

