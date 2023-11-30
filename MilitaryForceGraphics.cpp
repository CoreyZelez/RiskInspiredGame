#include "MilitaryForceGraphics.h"
#include "FontManager.h"
#include "MilitaryForce.h"

MilitaryForceGraphics::MilitaryForceGraphics(const sf::Texture &texture, const MilitaryForce &military)
	: SpriteGraphicsComponent(texture), military(military)
{
	// PUT CODE HERE TO STANDARDISE SIZE OF SPRITE IRRESPECTIVE OF TEXTURE!!!

	getSprite().setColor(sf::Color::White);
	setScale(sf::Vector2f(2, 2));

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
}

sf::Vector2f MilitaryForceGraphics::calculateTextPosition() const
{
	const int yTextAdjustment = 7;

	sf::FloatRect textBounds = text.getLocalBounds();
	sf::FloatRect spriteBounds = getSprite().getGlobalBounds();
	float x = spriteBounds.left + (spriteBounds.width - textBounds.width) / 2.0f;
	float y = spriteBounds.top + (spriteBounds.height / 2.0f) - textBounds.height + yTextAdjustment;
	return sf::Vector2f(x, y);
}

