#pragma once
#include <SFML/Graphics.hpp>

class UIPanel
{
public:
	UIPanel(sf::Color backgroundColor);
	virtual void draw(sf::RenderWindow &window) const;
	virtual void handleInteraction(sf::Vector2f position) = 0;
	virtual void setPosition(sf::Vector2f position);

protected:
	void resizeBackground(float width, float height);

private:
	sf::RectangleShape background;
};

