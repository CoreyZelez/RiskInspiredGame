#include "Territory.h"
#include <assert.h>



void Territory::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

bool Territory::isEmpty() const
{
	return graphics.isEmpty();
}

void Territory::addSquare(sf::Vector2f position)
{
	graphics.addSquare(position);
}

void Territory::removeSquare(sf::Vector2f position)
{
	graphics.removeSquare(position);
}

bool Territory::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}



