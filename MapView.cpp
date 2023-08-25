#include "MapView.h"
#include "Player.h"

MapView::MapView(const Map &map, const std::list<Player> &players)
	: map(map), players(players)
{
}

void MapView::draw(sf::RenderWindow &window) const
{
	switch(mode)
	{
	
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
