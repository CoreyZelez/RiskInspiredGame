#include "MilitaryForceGraphics.h"
#include "FontManager.h"
#include "MilitaryForce.h"
#include "TextureManager.h"
#include <assert.h>

MilitaryForceGraphics::MilitaryForceGraphics(const std::string &shape, const MilitaryForce &military)
	: SpriteGraphicsComponent(*getMilitaryShapeTexture(shape)), military(military), textOffset(getMilitaryShapeTextOffset(shape))
{
	// PUT CODE HERE TO STANDARDISE SIZE OF SPRITE IRRESPECTIVE OF TEXTURE!!!

	getSprite().setColor(sf::Color::White);
	setScale(sf::Vector2f(0.8, 0.8));

	FontManager &fontManager = FontManager::getInstance();

	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	std::string fontID = "militaryFont";
	text.setFont(*fontManager.getFont(fontID));
	text.setString(std::to_string(military.getTotalStrength()));
	text.setCharacterSize(80);
	text.setFillColor(sf::Color::White);
}


void MilitaryForceGraphics::draw(sf::RenderWindow & window) const
{
	SpriteGraphicsComponent::draw(window);
	window.draw(text);
}

void MilitaryForceGraphics::setPosition(sf::Vector2f position)
{
	SpriteGraphicsComponent::setPosition(position);

	text.setPosition(calculateTextPosition());
}

void MilitaryForceGraphics::update()
{
	text.setString(std::to_string(military.getTotalStrength()));
	text.setPosition(calculateTextPosition());
}

sf::Vector2f MilitaryForceGraphics::calculateTextPosition() const
{
	sf::FloatRect textBounds = text.getLocalBounds();
	sf::FloatRect spriteBounds = getSprite().getGlobalBounds();
	float x = spriteBounds.left + ((spriteBounds.width - textBounds.width) / 2.0f) + textOffset.x;
	float y = spriteBounds.top + ((spriteBounds.height) / 2.0f) - textBounds.height + textOffset.y;
	return sf::Vector2f(x, y);
}

const sf::Texture* MilitaryForceGraphics::getMilitaryShapeTexture(const std::string &shape)
{
	TextureManager &textureManager = TextureManager::getInstance();
	const sf::Texture *texture = nullptr;
	if(shape.compare("circle") == 0)
	{
		texture = textureManager.getTexture("circle");
	}
	else if(shape.compare("triangle") == 0)
	{
		texture = textureManager.getTexture("triangle");
	}
	assert(texture != nullptr);
	return texture;
}

sf::Vector2f MilitaryForceGraphics::getMilitaryShapeTextOffset(const std::string &shape)
{
	sf::Vector2f displacement(0, 0);

	if(shape.compare("triangle") == 0)
	{
		displacement = sf::Vector2f(0, 30);
	}
	else if(shape.compare("circle") == 0)
	{
		displacement = sf::Vector2f(-1, 4);
	}

	return displacement;
}


