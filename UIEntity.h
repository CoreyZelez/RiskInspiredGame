#pragma once
#include <SFML/Graphics.hpp>

class UIEntity
{
public:
	virtual void draw(sf::RenderWindow &window) const = 0;
	virtual void handleInteraction(sf::Vector2f position) = 0;
	virtual void setPosition(sf::Vector2f position) = 0;
};

