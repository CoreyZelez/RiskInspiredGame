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
sf::Vector2i nextPosition(const sf::Vector2i &position, Direction direction);


