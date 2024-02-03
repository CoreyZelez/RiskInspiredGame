#include "IntegerSlider.h"
#include <assert.h>
#include <iostream>

template<typename T>
IntegerSlider<T>::IntegerSlider(int minValue, int maxValue, T &value)
	: minValue(minValue), maxValue(maxValue), value(value), selectedAmount(value)
{
	assert(selectedAmount / divisor == value);

	sf::Color grey(150, 150, 150);
	sf::Color beige(200, 190, 150);

	backgroundBar.setFillColor(grey);
	selectionBar.setFillColor(beige);

	backgroundBar.setSize(sf::Vector2f(600, 80));
	selectionBar.setSize(sf::Vector2f(600, 80));

	updateSelectionBar();
}

template<typename T>
IntegerSlider<T>::IntegerSlider(int minValue, int maxValue, T &value, double divisor)
	: minValue(minValue), maxValue(maxValue), value(value), selectedAmount(value * divisor), divisor(divisor)
{
	sf::Color grey(150, 150, 150);
	sf::Color beige(200, 190, 150);

	assert(selectedAmount / divisor == value);

	backgroundBar.setFillColor(grey);
	selectionBar.setFillColor(beige);

	backgroundBar.setSize(sf::Vector2f(600, 80));
	selectionBar.setSize(sf::Vector2f(600, 80));

	updateSelectionBar();
}

template<typename T>
void IntegerSlider<T>::draw(sf::RenderWindow &window) const
{
	window.draw(backgroundBar);
	window.draw(selectionBar);
}

template<typename T>
void IntegerSlider<T>::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
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

		// Determine new selected amount and update value.
		selectedAmount = minValue + std::round(percent * (maxValue - minValue));
		updateValue();

		updateSelectionBar();
	}
}

template<typename T>
void IntegerSlider<T>::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect background = backgroundBar.getGlobalBounds();
		sf::Vector2f centerOffset = { -background.width / 2, -background.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}
	backgroundBar.setPosition(position);
	selectionBar.setPosition(position);
}

template<typename T>
sf::Vector2f IntegerSlider<T>::getDimensions()
{
	sf::FloatRect bounds = backgroundBar.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

template<typename T>
void IntegerSlider<T>::updateValue()
{
	value = (double)selectedAmount / divisor;
}

template<typename T>
void IntegerSlider<T>::updateSelectionBar()
{
	// Percent adjusted for rounded value.
	const float adjustedPercent = (float)(selectedAmount - minValue) / (maxValue - minValue);

	// Update size of selection bar.
	const float selectionBarWidth = backgroundBar.getGlobalBounds().width * adjustedPercent;
	const float selectionBarHeight = backgroundBar.getGlobalBounds().height;
	selectionBar.setSize(sf::Vector2f(selectionBarWidth, selectionBarHeight));
}
