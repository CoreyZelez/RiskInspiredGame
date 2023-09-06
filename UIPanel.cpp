#include "UIPanel.h"

UIPanel::UIPanel(sf::Color backgroundColor)
{
	background.setFillColor(backgroundColor);
}

void UIPanel::draw(sf::RenderWindow & window) const
{
	window.draw(background);
}

void UIPanel::setPosition(sf::Vector2f position)
{
	background.setPosition(position);
}

void UIPanel::resizeBackground(float width, float height)
{
	background.setSize(sf::Vector2f(width, height));
}
