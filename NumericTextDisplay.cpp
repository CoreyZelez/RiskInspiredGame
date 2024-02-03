#include "NumericTextDisplay.h"
#include "FontManager.h"
#include <iostream>

template<typename T>
NumericTextDisplay<T>::NumericTextDisplay(T &value)
	: NumericTextDisplay(value, "", "")
{
}

template<typename T>
inline NumericTextDisplay<T>::NumericTextDisplay(T &value, std::string prefix, std::string postfix)
	: value(value), prefix(prefix), postfix(postfix)
{
	// Initialise text.
	FontManager &fontManager = FontManager::getInstance();
	const sf::Font &font = *fontManager.getFont("UIFont1");
	text = sfe::RichText(font);
	text << sf::Text::Bold << prefix << std::to_string(value) << postfix;

	// Place UI at origin.
	setPosition(sf::Vector2f(0, 0));
}

template<typename T>
void NumericTextDisplay<T>::update()
{
	updateText();
}

template<typename T>
void NumericTextDisplay<T>::draw(sf::RenderWindow &window) const
{
	window.draw(background);
	window.draw(text);
}

template<typename T>
void NumericTextDisplay<T>::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
{
	// Intentionally empty.
}

template<typename T>
void NumericTextDisplay<T>::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect backgroundBounds = background.getGlobalBounds();
		sf::Vector2f centerOffset = { -backgroundBounds.width / 2, -backgroundBounds.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;		
	}
	background.setPosition(position);
	centerText();
}

template<typename T>
sf::Vector2f NumericTextDisplay<T>::getDimensions()
{
	sf::FloatRect bounds = background.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

template<typename T>
void NumericTextDisplay<T>::setBackgroundSize(sf::Vector2f dimensions)
{
	background.setSize(dimensions);
}

template<typename T>
void NumericTextDisplay<T>::setBackgroundColor(sf::Color color)
{
	background.setFillColor(color);
}

template<typename T>
void NumericTextDisplay<T>::setTextColor(sf::Color color)
{
	textColor = color;
	updateText();
}

template<typename T>
void NumericTextDisplay<T>::setCharacterSize(int characterSize)
{
	text.setCharacterSize(characterSize);
}

template<typename T>
void NumericTextDisplay<T>::updateText()
{
	text.clear();
	text << sf::Text::Regular << textColor << prefix << std::to_string(value) << postfix;
}

template<typename T>
void NumericTextDisplay<T>::centerText()
{
	// Get the position and size of the rectangle shape.
	sf::Vector2f backgroundPos = background.getPosition();
	sf::Vector2f backgroundSize = background.getSize();

	// Calculate the center of the rectangle shape.
	const float centerX = backgroundPos.x + backgroundSize.x / 2;
	const float centerY = backgroundPos.y + backgroundSize.y / 2;

	// Get the bounds of the text.
	sf::FloatRect textBounds = text.getLocalBounds();

	// Calculate the new position of the text.
	float newTextPosX = centerX - textBounds.width / 2;
	float newTextPosY = centerY - textBounds.height / 2;

	// Set the new position of the text.
	text.setPosition(sf::Vector2f(newTextPosX, newTextPosY));
}

