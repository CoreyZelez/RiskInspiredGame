#include "Direction.h"

Direction anticlockwiseTurn(Direction currDirection)
{
	switch(currDirection)
	{
	case Direction::up:
		return Direction::left;
	case Direction::left:
		return Direction::down;
	case Direction::down:
		return Direction::right;
	case Direction::right:
		return Direction::up;
	}
}

Direction operator++(Direction & d)
{
	switch(d)
	{
	case Direction::up:
		d = Direction::right;
		break;
	case Direction::right:
		d = Direction::down;
		break;
	case Direction::down:
		d = Direction::left;
		break;
	case Direction::left:
		d = Direction::up;
		break;
	}

	return d;
}

Direction operator--(Direction & d)
{
	switch(d)
	{
	case Direction::up:
		d = Direction::left;
		break;
	case Direction::left:
		d = Direction::down;
		break;
	case Direction::down:
		d = Direction::right;
		break;
	case Direction::right:
		d = Direction::up;
		break;
	}

	return d;
}

sf::Vector2i nextPosition(const sf::Vector2i & position, Direction direction)
{
	switch(direction)
	{
	case Direction::up:
		return sf::Vector2i(position.x, position.y - 1);
	case Direction::down:
		return sf::Vector2i(position.x, position.y + 1);
	case Direction::left:
		return sf::Vector2i(position.x - 1, position.y);
	case Direction::right:
		return sf::Vector2i(position.x + 1, position.y);
	}
}
;