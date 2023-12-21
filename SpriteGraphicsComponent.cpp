#include "SpriteGraphicsComponent.h"

SpriteGraphicsComponent::SpriteGraphicsComponent(const sf::Texture & texture)
{
	sprite.setTexture(texture);
}



void SpriteGraphicsComponent::draw(sf::RenderWindow & window) const
{
	window.draw(sprite);
}

void SpriteGraphicsComponent::update()
{
	// Intentionally empty.
}

void SpriteGraphicsComponent::setScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

void SpriteGraphicsComponent::setPosition(sf::Vector2f position)
{
	sf::Vector2f spritePos = position;
	spritePos.x -= sprite.getGlobalBounds().width / 2;
	spritePos.y -= sprite.getGlobalBounds().height / 2;
	sprite.setPosition(spritePos);
}

bool SpriteGraphicsComponent::containsPosition(sf::Vector2f position) const
{
	return sprite.getGlobalBounds().contains(position);
}

sf::Sprite& SpriteGraphicsComponent::getSprite()
{
	return sprite;
}

const sf::Sprite& SpriteGraphicsComponent::getSprite() const
{
	return sprite;
}

