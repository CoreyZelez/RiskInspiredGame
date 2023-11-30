#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteGraphicsComponent.h"

class MilitaryForce;

class MilitaryForceGraphics : public SpriteGraphicsComponent
{
public:
	MilitaryForceGraphics(const sf::Texture &texture, const MilitaryForce &military);

	// Draws the graphics.
	virtual void draw(sf::RenderWindow &window) const override;

	// Updates the graphics.
	virtual void update() override;

	// Changes position of the graphics.
	virtual void setPosition(sf::Vector2f position) override;

private:
	// Calculate the position for the text relative to the sprite.
	sf::Vector2f calculateTextPosition() const;

	const MilitaryForce &military; 

	sf::Text text;
	sf::Font font;
};

