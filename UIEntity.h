#pragma once
#include <SFML/Graphics.hpp>

class UIEntity
{
public:
	virtual void draw(sf::RenderWindow &window) const = 0;
	virtual void handleButtonClick(sf::Mouse::Button, sf::Vector2f position) {};
	virtual void handleButtonDown(sf::Mouse::Button, sf::Vector2f position) {};
	virtual void setPosition(sf::Vector2f position) = 0;
};

