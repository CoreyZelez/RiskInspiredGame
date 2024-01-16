#include "IntegerSlider.h"
#include <assert.h>
IntegerSlider::IntegerSlider(int minValue, int maxValue)
	: minValue(minValue), maxValue(maxValue)
{
	sf::Color grey(150, 150, 150);
	sf::Color darkGrey(220, 220, 220);

	backgroundBar.setFillColor(sf::Color(grey));
	selectionBar.setFillColor(darkGrey);
}

void IntegerSlider::draw(sf::RenderWindow &window) const
{
	window.draw(backgroundBar);
	window.draw(selectionBar);
}

void IntegerSlider::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
{
	if(button != sf::Mouse::Button::Left)
	{
		return;
	}

	if(backgroundBar.getGlobalBounds().contains(position))
	{
		const float left = backgroundBar.getGlobalBounds().left;
		const float width = backgroundBar.getGlobalBounds().width;
		const float xSelection = position.x;
		// Percent of bar to be filled.
		const float percent = (xSelection - left) / width;
		assert(0 <= percent <= 1);
		selectedAmount = minValue + std::round(percent * (maxValue - minValue));
	}
}

void IntegerSlider::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect background = backgroundBar.getLocalBounds();
		sf::Vector2f centerOffset = { -background.width / 2, -background.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}
	backgroundBar.setPosition(position);
	selectionBar.setPosition(position);
}

sf::Vector2f IntegerSlider::getDimensions()
{
	sf::FloatRect bounds = backgroundBar.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}
