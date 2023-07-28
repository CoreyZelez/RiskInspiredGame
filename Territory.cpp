#include "Territory.h"
#include <assert.h>



void Territory::draw(sf::RenderWindow &window) const
{
	graphics.draw(window);
}

void Territory::addGridPosition(sf::Vector2f position)
{
	graphics.addGridPosition(position);
}

double Territory::getDefenceMultiplier() const
{
	return defenceMultiplier;
}



