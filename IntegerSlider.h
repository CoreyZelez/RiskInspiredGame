#pragma once
#include "UIEntity.h"

template<typename T> class IntegerSlider : public UIEntity
{
public:
	IntegerSlider(int minValue, int maxValue, T &value);
	IntegerSlider(int minValue, int maxValue, T &value, double divisor);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

private:
	const int minValue;
	const int maxValue;
	int selectedAmount;
	T &value;  // Value being modified.
	double divisor = 1;  // Divisor of slider selected amount applied before altering value.
	sf::RectangleShape backgroundBar;
	sf::RectangleShape selectionBar;

	void updateValue();
};

