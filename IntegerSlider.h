#pragma once
#include "UIEntity.h"

class IntegerSlider : public UIEntity
{
public:
	IntegerSlider(int minValue, int maxValue);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

private:
	const int minValue;
	const int maxValue;
	int selectedAmount;
	sf::RectangleShape backgroundBar;
	sf::RectangleShape selectionBar;
};


