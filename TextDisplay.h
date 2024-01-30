#pragma once
#include "UIEntity.h"
#include "RichText.h"

/*
 * Displays text relating to some primitive type within a box.
 */
template<typename T> class TextDisplay : public UIEntity
{
public:
	TextDisplay(T &value, std::string prefix, std::string postfix, sf::Color textColor, int characterSize, sf::Color backgroundColor, float padding);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

private:
	const float padding;
	sf::RectangleShape background;
	sfe::RichText text;
	const T& value;
	const std::string prefix;
	const std::string postfix;
};

#include "TextDisplay.h"

