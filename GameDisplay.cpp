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

	drawPorts(window);
}

void GameDisplay::drawMilitaries(sf::RenderWindow & window) const
{
	if(game.getDisplayOptions().displayMilitaries)
	{
		for(auto &player : players)
		{
			player.get()->getMilitaryManager().draw(window);
		}
	}
}

void GameDisplay::drawPorts(sf::RenderWindow & window) const
{
	if(game.getDisplayOptions().displayPorts)
	{
		game.getMap().getTerritoryManager().drawPorts(window);
	}
}

