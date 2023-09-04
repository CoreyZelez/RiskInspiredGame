#include "GameView.h"
#include "Game.h"

GameView::GameView(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>>& players)
	: game(game), mapView(game, map, players), players(players)
{
}

void GameView::draw(sf::RenderWindow & window) const
{
	mapView.draw(window);

	drawMilitaries(window);

	gameUI.draw(window);
}

void GameView::setLeftPanel(const IHasUI &hasUI)
{
	gameUI.setLeftPanel(hasUI);
}

void GameView::setRightPanel(const IHasUI & hasUI)
{
	gameUI.setRightPanel(hasUI);
}

void GameView::setBottomPanel(const IHasUI & hasUI)
{
	gameUI.setBottomPanel(hasUI);
}

void GameView::setMapMode(MapMode mapMode)
{
	mapView.setMapMode(mapMode);
}

MapMode GameView::getMapMode() const
{
	return mapView.getMapMode();
}

void GameView::drawMilitaries(sf::RenderWindow & window) const
{
	if(game.getDisplayMilitary())
	{
		for(auto &player : players)
		{
			player.get()->getMilitaryManager().draw(window);
		}
	}
}

