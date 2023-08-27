#pragma once
#include <SFML/Graphics.hpp>

class MilitaryForceGraphics
{
public:
	MilitaryForceGraphics(const sf::Texture &texture, const int &strength);

	void draw(sf::RenderWindow &window) const;
	void setPosition(sf::Vector2f position);
	void update();

private:
	// Calculate the position for the text relative to the sprite.
	sf::Vector2f calculateTextPosition() const;

	const int &strength;  // Strength of army.

	sf::Sprite sprite;
	sf::Text text;
	sf::Font font;
};

