#include "StrengthSlider.h"
#include "MilitaryForce.h"
#include <assert.h>
#include <iostream>
StrengthSlider::StrengthSlider(const MilitaryForce & militaryForce, unsigned int &selectedStrength)
	: militaryForce(militaryForce), selectedStrength(selectedStrength)
{
	sf::Color green(0, 235, 0);
	sf::Color yellow(215, 215, 0);
	sf::Color orange(215, 107, 0);
	sf::Color red(225, 0, 0);

	backgroundBar.setFillColor(sf::Color(40, 40, 40));
	greenBar.setFillColor(green);
	yellowBar.setFillColor(yellow);
	orangeBar.setFillColor(orange);
	redBar.setFillColor(red);
}

void StrengthSlider::draw(sf::RenderWindow &window) const
{
	window.draw(backgroundBar);
	window.draw(redBar);
	window.draw(orangeBar);
	window.draw(yellowBar);
	window.draw(greenBar);
}

void StrengthSlider::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
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
		const int newSelectedStrength = std::round(percent * militaryForce.getTotalStrength());
		updateSelectedStrength(newSelectedStrength);
	}
}

void StrengthSlider::setPosition(sf::Vector2f position, bool center)
{
	if(center)
	{
		sf::FloatRect background = backgroundBar.getLocalBounds();
		sf::Vector2f centerOffset = { -background.width / 2, -background.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}
	backgroundBar.setPosition(position);
	updateBars();
}

sf::Vector2f StrengthSlider::getDimensions()
{
	sf::FloatRect bounds = backgroundBar.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void StrengthSlider::updateSelectedStrength(const int amount)
{
	selectedStrength = amount;
	updateBars();
}

void StrengthSlider::updateBars()
{
	const float maxWidth = backgroundBar.getGlobalBounds().width;
	const int totalStrength = militaryForce.getTotalStrength();
	int remainingStrength = selectedStrength;
	const int greenStrength = militaryForce.getStaminaStrength()[3];
	const int yellowStrength = militaryForce.getStaminaStrength()[2];
	const int orangeStrength = militaryForce.getStaminaStrength()[1];
	const int redStrength = militaryForce.getStaminaStrength()[0];
	assert(greenStrength + yellowStrength + orangeStrength + redStrength
		== militaryForce.getTotalStrength());

	// Sets green bar.
	if(remainingStrength > greenStrength)
	{
		remainingStrength -= greenStrength;
		const float strengthRatio = (float)greenStrength / (float)totalStrength;
		const float widthGreen = maxWidth * strengthRatio;
		greenBar.setPosition(backgroundBar.getPosition());
		greenBar.setSize(sf::Vector2f(widthGreen, backgroundBar.getSize().y));
	}
	else
	{
		const float strengthRatio = (float)remainingStrength / (float)totalStrength;
		remainingStrength = 0;
		const float widthGreen = maxWidth * strengthRatio;
		greenBar.setPosition(backgroundBar.getPosition());
		greenBar.setSize(sf::Vector2f(widthGreen, backgroundBar.getSize().y));
	}

	// Sets yellow bar.
	if(remainingStrength > yellowStrength)
	{
		remainingStrength -= yellowStrength;
		const float strengthRatio = (float)yellowStrength / (float)totalStrength;
		const float widthYellow = maxWidth * strengthRatio;
		const sf::Vector2f yellowPosition(greenBar.getPosition().x + greenBar.getSize().x, backgroundBar.getPosition().y);
		yellowBar.setPosition(yellowPosition);
		yellowBar.setSize(sf::Vector2f(widthYellow, backgroundBar.getSize().y));
	}
	else
	{
		const float strengthRatio = (float)remainingStrength / (float)totalStrength;
		remainingStrength = 0;
		const float widthYellow = maxWidth * strengthRatio;
		const sf::Vector2f yellowPosition(greenBar.getPosition().x + greenBar.getSize().x, backgroundBar.getPosition().y);
		yellowBar.setPosition(yellowPosition);
		yellowBar.setSize(sf::Vector2f(widthYellow, backgroundBar.getSize().y));
	}

	// Sets orange bar.
	if(remainingStrength > orangeStrength)
	{
		remainingStrength -= orangeStrength;
		const float strengthRatio = (float)orangeStrength / (float)totalStrength;
		const float widthOrange = maxWidth * strengthRatio;
		const sf::Vector2f orangePosition(yellowBar.getPosition().x + yellowBar.getSize().x, backgroundBar.getPosition().y);
		orangeBar.setPosition(orangePosition);
		orangeBar.setSize(sf::Vector2f(widthOrange, backgroundBar.getSize().y));
	}
	else
	{
		const float strengthRatio = (float)remainingStrength / (float)totalStrength;
		remainingStrength = 0;
		const float widthOrange = maxWidth * strengthRatio;
		const sf::Vector2f orangePosition(yellowBar.getPosition().x + yellowBar.getSize().x, backgroundBar.getPosition().y);
		orangeBar.setPosition(orangePosition);
		orangeBar.setSize(sf::Vector2f(widthOrange, backgroundBar.getSize().y));
	}

	// Sets red bar.
	if(remainingStrength > redStrength)
	{
		remainingStrength -= orangeStrength;
		const float strengthRatio = (float)redStrength / (float)totalStrength;
		const float widthRed = maxWidth * strengthRatio;
		const sf::Vector2f redPosition(orangeBar.getPosition().x + orangeBar.getSize().x, backgroundBar.getPosition().y);
		redBar.setPosition(redPosition);
		redBar.setSize(sf::Vector2f(widthRed, backgroundBar.getSize().y));
	}
	else
	{
		const float strengthRatio = (float)remainingStrength / (float)totalStrength;
		remainingStrength = 0;
		const float widthRed = maxWidth * strengthRatio;
		const sf::Vector2f redPosition(orangeBar.getPosition().x + orangeBar.getSize().x, backgroundBar.getPosition().y);
		redBar.setPosition(redPosition);
		redBar.setSize(sf::Vector2f(widthRed, backgroundBar.getSize().y));
	}
}

