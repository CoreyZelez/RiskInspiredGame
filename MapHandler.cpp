#include "MapHandler.h"
#include "Player.h"

MapHandler::MapHandler(std::string name, const std::list<Player> &players)
	: map(name), players(players)
{
}

void MapHandler::draw(sf::RenderWindow &window) const
{
	switch(mode)
	{
	
	}
}

void MapHandler::changeMapMode(MapMode mode)
{
	this->mode = mode;
	
	switch(mode)
	{
	case MapMode::realm:
	
		break;
	}
}
