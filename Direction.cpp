#include "Direction.h"
#include <assert.h>

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

sf::Vector2i determineNextPosition(const sf::Vector2i & position, Direction direction)
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

Direction getRelativeDirection(const sf::Vector2i &pos1, const sf::Vector2i &pos2)
{
	const int xDiff = pos2.x - pos1.x;
	const int yDiff = pos2.y - pos1.y;

	assert((xDiff != 0 && yDiff == 0) || (xDiff == 0 && yDiff != 0));

	if(xDiff > 0)
	{
		return Direction::right;
	}
	else if(xDiff < 0)
	{
		return Direction::left;
	}
	else if(yDiff > 0)
	{
		return Direction::down;
	}
	else
	{
		return Direction::up;
	}
}

Direction getLeftTurnsDirection(const Direction& direction, int leftTurns)
{
	Direction newDirection = direction;

	for (int i = 0; i < leftTurns; ++i)
	{
		--newDirection;
	}

	return newDirection;
}

Direction getRightTurnsDirection(const Direction& direction, int rightTurns)
{
	Direction newDirection = direction;

	for (int i = 0; i < rightTurns; ++i)
	{
		++newDirection;
	}

	return newDirection;
}
