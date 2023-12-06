#pragma once
#include <SFML/Graphics.hpp>

class GraphicsComponent
{
public:
	// Draws the graphics.
	virtual void draw(sf::RenderWindow &window) const = 0;

	// Updates the graphics.
	virtual void update() = 0;

	// Changes position of the graphics.
	virtual void setPosition(sf::Vector2f position) = 0;

	// Scales the graphics.
	virtual void setScale(sf::Vector2f scale) = 0;

	// Returns true if graphics contains specified position.
	virtual bool containsPosition(sf::Vector2f position) const = 0;

private:

};

