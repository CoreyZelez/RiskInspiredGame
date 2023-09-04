#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class UIDecoration {
public:
	UIDecoration(const std::string& text, const sf::Font& font, 
		unsigned int fontSize, const sf::Color& textColor, const sf::Color& backgroundColor);

	void setPosition(float x, float y);
	void setText(const std::string& text);
	sf::FloatRect getTextBounds() const;

	void draw(sf::RenderWindow& window) const;  // Mark draw as const

private:
	sf::RectangleShape background;
	sf::Text textObject;
};