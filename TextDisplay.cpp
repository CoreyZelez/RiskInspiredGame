#include "TextDisplay.h"
#include "FontManager.h"

template<typename T>
inline TextDisplay<T>::TextDisplay(T &value, std::string prefix, std::string postfix, sf::Color textColor, int characterSize, sf::Color backgroundColor, float padding)
	: value(value), prefix(prefix), postfix(postfix), padding(padding)
{
	// Initialise text.
	FontManager &fontManager = FontManager::getInstance();
	const sf::Font &font = *fontManager.getFont("UIFont1");
	sfe::RichText text(font);
	text.setCharacterSize(characterSize);
	text << sf::Text::Regular << textColor << prefix << std::to_string(value) << postfix;
	text.setPosition(padding, padding);

	// Initialise background rectangle.
	background.setFillColor(backgroundColor);
	const float backgroundWidth = text.getGlobalBounds().width + 2 * padding;
	const float backgroundHeight = text.getGlobalBounds().width + 2 * padding;
	background = sf::RectangleShape(sf::Vector2f(backgroundWidth, backgroundHeight));
	background.setPosition(0, 0);
}

template<typename T>
void TextDisplay<T>::draw(sf::RenderWindow & window) const
{
	window.draw(background);
	window.draw(text);
}

template<typename T>
void TextDisplay<T>::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
{
	// Intentionally empty.
}

template<typename T>
void TextDisplay<T>::setPosition(sf::Vector2f position, bool center)
{
	sf::Vector2f textPosition = sf::Vector2f(position.x + padding, position.y + padding);
	if(center)
	{
		sf::FloatRect backgroundBounds = background.getGlobalBounds();
		sf::Vector2f centerOffset = { -backgroundBounds.width / 2, -backgroundBounds.height / 2 };
		position.x += centerOffset.x;
		textPosition.x += centerOffset.x;
		position.y += centerOffset.y;		
		textPosition.y += centerOffset.y;
	}
	background.setPosition(position);
	text.setPosition(textPosition);
}

template<typename T>
sf::Vector2f TextDisplay<T>::getDimensions()
{
	sf::FloatRect bounds = background.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}
