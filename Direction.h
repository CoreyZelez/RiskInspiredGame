#pragma once
#include <SFML/Graphics.hpp>

enum class Direction
{
	up,
	right,
	down,
	left
};

Direction operator++(Direction &d);

Direction operator--(Direction &d);

// Returns the next position in the specified direction.
sf::Vector2i determineNextPosition(const sf::Vector2i &position, Direction direction);

// Returns direction of pos2 relative to pos1.
Direction getRelativeDirection(const sf::Vector2i &pos1, const sf::Vector2i &pos2);

// Returns direction after applying a specific number of 90 degree left (anticlockwise) turns.
Direction getLeftTurnsDirection(const Direction& direction, int leftTurns);

// Returns direction after applying a specific number of 90 degree right (clockwise) turns.
Direction getRightTurnsDirection(const Direction& direction, int rightTurns);

