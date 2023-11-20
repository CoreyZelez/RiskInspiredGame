#pragma once
#include <SFML/Graphics.hpp>

class MilitaryForce;

class MilitaryForceGraphics
{
public:
	MilitaryForceGraphics(const sf::Texture &texture, const MilitaryForce &military);

	bool containsPosition(sf::Vector2f position) const;
	void draw(sf::RenderWindow &window) const;
	void setPosition(sf::Vector2f position);
	void update();

private:
	// Calculate the position for the text relative to the sprite.
	sf::Vector2f calculateTextPosition() const;

	const MilitaryForce &military; 

	sf::Sprite sprite;
	sf::Text text;
	sf::Font font;
};

