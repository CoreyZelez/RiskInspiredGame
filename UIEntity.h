#pragma once
#include <SFML/Graphics.hpp>

class UIEntity
{
public:
	virtual void update() {};
	virtual void draw(sf::RenderWindow &window) const = 0;
	virtual void handleButtonClick(sf::Mouse::Button, sf::Vector2f position) {};
	virtual void handleButtonDown(sf::Mouse::Button, sf::Vector2f position) {};
	// Sets position of UI entity. Optionally specify position of center.
	virtual void setPosition(sf::Vector2f position, bool center = false) = 0;
	// Returns minimal width and height of rectangular space UI entity occupies.
	virtual sf::Vector2f getDimensions() = 0;

};

