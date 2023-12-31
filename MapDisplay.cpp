#include "MapDisplay.h"
#include "Player.h"
#include "Game.h"

MapDisplay::MapDisplay(const Game & game, const Map & map, const std::vector<std::unique_ptr<Player>>& players)
	: game(game), map(map), players(players)
{
}

void MapDisplay::draw(sf::RenderWindow &window) const
{
	switch(game.getMapMode())
	{
	case MapMode::selectedRealm:
		map.getEstateManager().drawUnownedMaridoms(window);
		for(const auto &player : players)
		{
			if(!player.get()->hasLiege())
			{
				player.get()->getRealm().draw(window);
			}
		}
		break;
	case MapMode::realm:
		map.getEstateManager().drawUnownedMaridoms(window);
		for(const auto &player : players)
		{
			if(!player.get()->hasLiege())
			{
				player.get()->getRealm().draw(window);
			}
		}
		break;
	case MapMode::county:
		map.getEstateManager().draw(window, Title::county);
		break;
	case MapMode::duchy:
		map.getEstateManager().draw(window, Title::duchy);
		break;
	case MapMode::kingdom:
		map.getEstateManager().draw(window, Title::kingdom);
		break;
	case MapMode::empire:
		map.getEstateManager().draw(window, Title::empire);
		break;
	}
}

