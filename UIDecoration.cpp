#include "UIDecoration.h"

const float padding = 5.0f;  // Adjust this value as needed for spacing

UIDecoration::UIDecoration(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color& textColor, const sf::Color& backgroundColor) {
	textObject.setFont(font);
	textObject.setCharacterSize(fontSize);
	textObject.setFillColor(textColor);
	setText(text);

	sf::FloatRect textBounds = textObject.getLocalBounds();
	background.setSize(sf::Vector2f(textBounds.width + 2 * padding, textBounds.height + 2 * padding));
	background.setFillColor(backgroundColor);
}

void UIDecoration::setPosition(float x, float y) {
	background.setPosition(x, y);
	textObject.setPosition(x + padding, y + padding);
}

void UIDecoration::setText(const std::string& text) {
	textObject.setString(text);
}

sf::FloatRect UIDecoration::getTextBounds() const {
	return textObject.getGlobalBounds();
}

void UIDecoration::draw(sf::RenderWindow& window) const {
	window.draw(background);
	window.draw(textObject);
}


