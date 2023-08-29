#include "MilitaryForceGraphics.h"

MilitaryForceGraphics::MilitaryForceGraphics(const sf::Texture &texture, const int &strength)
	: strength(strength)
{
	// PUT CODE HERE TO STANDARDISE SIZE OF SPRITE IRRESPECTIVE OF TEXTURE!!!

	sprite.setTexture(texture);
	sprite.setColor(sf::Color::White);
	sprite.setScale(sf::Vector2f(2, 2));

	if(!font.loadFromFile("res/fonts/ChunkFive-Regular.otf"))
	{
		exit(1);
	}

	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setFont(font);
	text.setString(std::to_string(strength));
	text.setCharacterSize(80);
	text.setFillColor(sf::Color::White);
}

bool MilitaryForceGraphics::containsPosition(sf::Vector2f position) const
{
	return sprite.getGlobalBounds().contains(position);
}

void MilitaryForceGraphics::draw(sf::RenderWindow & window) const
{
	window.draw(sprite);
	window.draw(text);
}

void MilitaryForceGraphics::setPosition(sf::Vector2f position)
{
	sf::Vector2f spritePos = position;
	spritePos.x -= sprite.getGlobalBounds().width / 2;
	spritePos.y -= sprite.getGlobalBounds().height / 2;
	sprite.setPosition(spritePos);

	// text.setPosition(position);
	text.setPosition(calculateTextPosition());
}

void MilitaryForceGraphics::update()
{
	text.setString(std::to_string(strength));
}

sf::Vector2f MilitaryForceGraphics::calculateTextPosition() const
{
	const int yTextAdjustment = 7;

	sf::FloatRect textBounds = text.getLocalBounds();
	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	float x = spriteBounds.left + (spriteBounds.width - textBounds.width) / 2.0f;
	float y = spriteBounds.top + (spriteBounds.height / 2.0f) - textBounds.height + yTextAdjustment;
	return sf::Vector2f(x, y);
}

