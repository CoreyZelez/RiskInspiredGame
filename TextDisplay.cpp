#include "TextDisplay.h"
#include "FontManager.h"

TextDisplay::TextDisplay(std::string text)
	: text(text)
{
	// Initialise text.
	FontManager &fontManager = FontManager::getInstance();
	const sf::Font &font = *fontManager.getFont("UIFont1");
	richText = sfe::RichText(font);
	richText << sf::Text::Bold << text;

	// Place UI at origin.
	setPosition(sf::Vector2f(0, 0));
}

void TextDisplay::draw(sf::RenderWindow & window) const
{
	window.draw(background);
	window.draw(richText);
}

void TextDisplay::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect backgroundBounds = background.getGlobalBounds();
		sf::Vector2f centerOffset = { -backgroundBounds.width / 2, -backgroundBounds.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}
	background.setPosition(position);

	if(alignment == Alignment::left)
	{
		leftAlign();
	}
	else if(alignment == Alignment::center)
	{
		centerText();
	}
	else if(alignment == Alignment::right)
	{
		// Implement.
	}
}

sf::Vector2f TextDisplay::getDimensions()
{
	sf::FloatRect bounds = background.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void TextDisplay::setBackgroundSize(sf::Vector2f dimensions)
{
	background.setSize(dimensions);
}

void TextDisplay::setBackgroundColor(sf::Color color)
{
	background.setFillColor(color);
}

void TextDisplay::setCharacterSize(int characterSize)
{
	richText.setCharacterSize(characterSize);
}

void TextDisplay::setTextColor(sf::Color color)
{
	richText << sf::Text::Regular << color << text;
}

void TextDisplay::setPadding(float padding)
{
	this->padding = padding;
}

void TextDisplay::setAlignment(Alignment alignment)
{
	this->alignment = alignment;
}

void TextDisplay::centerText()
{
	// Get the position and size of the rectangle shape.
	sf::Vector2f backgroundPos = background.getPosition();
	sf::Vector2f backgroundSize = background.getSize();

	// Calculate the center of the rectangle shape.
	const float centerX = backgroundPos.x + backgroundSize.x / 2;
	const float centerY = backgroundPos.y + backgroundSize.y / 2;

	// Get the bounds of the text.
	sf::FloatRect textBounds = richText.getLocalBounds();

	// Calculate the new position of the text.
	float newTextPosX = centerX - textBounds.width / 2;
	float newTextPosY = centerY - textBounds.height / 2;

	// Set the new position of the text.
	richText.setPosition(sf::Vector2f(newTextPosX, newTextPosY));
}

void TextDisplay::leftAlign()
{
	// Center the text on the background to determine the y position.
	centerText();
	const float x = background.getGlobalBounds().left + padding;
	const float y = richText.getPosition().y;
	richText.setPosition(x, y);
}
