#pragma once
#include "GraphicsComponent.h"

class SpriteGraphicsComponent : public GraphicsComponent
{
public:
	SpriteGraphicsComponent(const sf::Texture &texture);

	// Draws the graphics.
	virtual void draw(sf::RenderWindow &window) const override;

	// Updates the graphics.
	virtual void update() override;

	// Scales the graphics.
	virtual void setScale(sf::Vector2f scale) override;

	// Changes position of the graphics to be central at position.
	virtual void setPosition(sf::Vector2f position) override;

	// Returns true if graphics contains specified position.
	virtual bool containsPosition(sf::Vector2f position) const;

protected:
	sf::Sprite &getSprite();
	const sf::Sprite &getSprite() const;

private:
	sf::Sprite sprite;
};

