#include "MapView.h"
#include "Player.h"

MapView::MapView(const Map &map, const std::vector<std::unique_ptr<Player>> &players)
	: map(map), players(players)
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
	}

	if(displayMilitary)
	{
		for(auto & player : players)
		{
			player.get()->getMilitaryManager().draw(window);
		}
	}
}

void MapView::changeMapMode(MapMode mode)
{
	this->mode = mode;
	
	switch(mode)
	{
	case MapMode::realm:
	
		break;
	}
}

void MapView::changeDisplayMilitary()
{
	displayMilitary = !displayMilitary;
}
