#include "Territory.h"
#include <assert.h>
#include <iostream>


Territory::Territory(sf::Color color)
	: graphics(color)
{
}

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

bool Territory::sharesBorder(const Territory &territory) const
{
	return graphics.sharesBorder(territory.graphics);
}

bool Territory::containsPosition(sf::Vector2f position) const
{
	return graphics.containsPosition(position);
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}

sf::Vector2f Territory::getCenter() const
{
	return graphics.getCenter();
}



