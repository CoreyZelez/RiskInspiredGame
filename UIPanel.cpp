#include "UIPanel.h"

UIPanel::UIPanel()
	: UIPanel(sf::Color(30, 30, 30))
{
}

UIPanel::UIPanel(sf::Color backgroundColor)
{
	background.setFillColor(backgroundColor);
}

void UIPanel::draw(sf::RenderWindow & window) const
{
	window.draw(background);
}

void UIPanel::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect background = getBackground().getGlobalBounds();
		sf::Vector2f centerOffset = { -background.width / 2, -background.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}

	background.setPosition(position);
}

sf::Vector2f UIPanel::getDimensions()
{
	sf::FloatRect bounds = background.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

const sf::RectangleShape& UIPanel::getBackground() const
{
	return background;
}

void UIPanel::resizeBackground(float width, float height)
{
	background.setSize(sf::Vector2f(width, height));
}
