#include "MapView.h"
#include "Player.h"
#include "Game.h"

MapView::MapView(const Game &game, const Map &map, const std::vector<std::unique_ptr<Player>> &players)
	: game(game), map(map), players(players)
{
}

void MapView::draw(sf::RenderWindow &window) const
{
	switch(mode)
	{
	case MapMode::realm:
		for(const auto &player : players)
		{
			if(!player.get()->getRealm().hasLiege())
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

void MapView::setMapMode(MapMode mode)
{
	this->mode = mode;
	
	switch(mode)
	{
	case MapMode::realm:
	
		break;
	}
}

MapMode MapView::getMapMode() const
{
	return mode;
}

