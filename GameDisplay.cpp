#include "GameDisplay.h"
#include "Game.h"

GameDisplay::GameDisplay(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>>& players)
	: game(game), MapDisplay(game, map, players), players(players)
{
}

void GameDisplay::draw(sf::RenderWindow &window) const
{
	MapDisplay.draw(window);

	drawMilitaries(window);
}

void GameDisplay::drawMilitaries(sf::RenderWindow & window) const
{
	if(game.getDisplayMilitary())
	{
		for(auto &player : players)
		{
			player.get()->getMilitaryManager().draw(window);
		}
	}
}

