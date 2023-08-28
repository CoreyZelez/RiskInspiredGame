#include "GameView.h"

GameView::GameView(const Map &map, const std::vector<std::unique_ptr<Player>>& players)
	: mapView(map, players)
{
}

void GameView::draw(sf::RenderWindow & window) const
{
	mapView.draw(window);
	gameUI.draw(window);
}

void GameView::setMapMode(MapMode mapMode)
{
	mapView.setMapMode(mapMode);
}

