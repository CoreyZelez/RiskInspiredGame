#include "GameView.h"

GameView::GameView(const Map &map, const std::list<Player>& players)
	: mapView(map, players)
{
}

void GameView::draw(sf::RenderWindow & window) const
{
	mapView.draw(window);
	gameUI.draw(window);
}

