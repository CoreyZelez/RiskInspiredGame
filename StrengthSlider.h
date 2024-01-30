#pragma once
#include "UIEntity.h"

class MilitaryForce;

class StrengthSlider : public UIEntity
{
public:
	explicit StrengthSlider(const MilitaryForce &militaryForce, unsigned int &selectedStrength);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

private:
	unsigned int &selectedStrength;
	const MilitaryForce &militaryForce;
	sf::RectangleShape backgroundBar = sf::RectangleShape(sf::Vector2f(1000, 100));
	sf::RectangleShape greenBar;
	sf::RectangleShape yellowBar;
	sf::RectangleShape orangeBar;
	sf::RectangleShape redBar;

	void updateSelectedStrength(const int amount);
	void updateBars();
};

