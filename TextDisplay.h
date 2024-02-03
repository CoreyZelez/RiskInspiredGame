#pragma once
#include "UIEntity.h"
#include "RichText.h"

enum class Alignment
{
	left,
	center,
	right
};

class TextDisplay : public UIEntity
{
public:
	TextDisplay(std::string text);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;
	virtual sf::Vector2f getDimensions() override;

	void setBackgroundSize(sf::Vector2f dimensions);
	void setBackgroundColor(sf::Color color);

	void setCharacterSize(int characterSize);
	void setTextColor(sf::Color color);

	void setPadding(float padding);
	void setAlignment(Alignment alignment);
	

private:
	sf::RectangleShape background;
	sfe::RichText richText;
	std::string text;
	Alignment alignment = Alignment::left;
	float padding = 0;  // Padding for left and right alignment of text from background edge.

	// Centers text on background.
	void centerText();
	// Left aligns text on background.
	void leftAlign();
};

