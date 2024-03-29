#pragma once
#include "UIEntity.h"
#include "RichText.h"

// Displays text relating to some primitive type within a box.
template<typename T> class NumericTextDisplay : public UIEntity
{
public:
	NumericTextDisplay(T &value, double multiplier = 1);
	NumericTextDisplay(T &value, std::string prefix, std::string postfix, double multiplier = 1);

	virtual void update() override;
	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

	void setBackgroundSize(sf::Vector2f dimensions);
	void setBackgroundColor(sf::Color color);

	void setTextColor(sf::Color color);
	void setCharacterSize(int characterSize);

	void setNumDecimals(int numDecimals);

private:
	sf::RectangleShape background;
	sfe::RichText text;
	sf::Color textColor;
	const T& value;
	double multiplier = 1;
	int numDecimals = 0;
	const std::string prefix;
	const std::string postfix;

	// Redetermines the text for changes to value.
	void updateText();

	// Centers text on the background.
	void centerText();
};


