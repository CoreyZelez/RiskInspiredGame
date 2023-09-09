#include "MapDisplay.h"
#include "Player.h"
#include "Game.h"

MapDisplay::MapDisplay(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>> &players)
	: game(game), map(map), players(players)
{
}

void MapDisplay::draw(sf::RenderWindow &window) const
{
	switch(game.getMapMode())
	{
	case MapMode::realm:
		for(const auto &player : players)
		{
			if(!player.get()->getRealm().getRelationshipManager().hasLiege())
			{
				player.get()->getRealm().draw(window);
			}
		}
		break;
	case MapMode::county:
		map.getEstateManager().draw(window, Title::count);
		break;
	case MapMode::duchy:
		map.getEstateManager().draw(window, Title::duke);
		break;
	case MapMode::kingdom:
		map.getEstateManager().draw(window, Title::king);
		break;
	case MapMode::empire:
		map.getEstateManager().draw(window, Title::emperor);
		break;
	}
}

