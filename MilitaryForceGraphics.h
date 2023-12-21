#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteGraphicsComponent.h"

class MilitaryForce;

class MilitaryForceGraphics : public SpriteGraphicsComponent
{
public:
	MilitaryForceGraphics(const std::string &shape, const MilitaryForce &military);

	// Draws the graphics.
	virtual void draw(sf::RenderWindow &window) const override;

	// Updates the graphics.
	virtual void update() override;

	// Changes position of the graphics.
	virtual void setPosition(sf::Vector2f position) override;

private:
	const MilitaryForce &military; 
	sf::Text text;
	const sf::Vector2f textOffset;
	sf::Font font;

	// Calculate the position for the text relative to the sprite.
	sf::Vector2f calculateTextPosition() const;

	static const sf::Texture* getMilitaryShapeTexture(const std::string &shape);

	static sf::Vector2f getMilitaryShapeTextOffset(const std::string &shape);
};

