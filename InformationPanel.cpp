#include "InformationPanel.h"
#include "FontManager.h"
#include <assert.h>
#include <iostream>

InformationPanel::InformationPanel(std::vector<sfe::RichText> texts)
	: UIPanel(sf::Color(30, 30, 30, 225)), texts(texts)
{
	float backgroundWidth = 0;
	float backgroundHeight = 0; 
	for(sfe::RichText &text : this->texts)
	{
		text.setCharacterSize(27);  
	
		const float width = text.getGlobalBounds().width;
		if(width > backgroundWidth)
		{
			backgroundWidth = width;
		}
	
		backgroundHeight += text.getGlobalBounds().height;
	}
	
	// Add paddings to background height and width.
	backgroundWidth += 2 * borderPadding;
	backgroundHeight += 2 * borderPadding + (textPadding * (this->texts.size() - 1));
	
	resizeBackground(backgroundWidth, backgroundHeight);
	
	assert(backgroundWidth > 0);
}

void InformationPanel::draw(sf::RenderWindow &window) const
{
	UIPanel::draw(window);

	for(const sfe::RichText &text : texts)
	{
		window.draw(text);
	}
}

void InformationPanel::setPosition(sf::Vector2f position, bool center)
{
	UIPanel::setPosition(position, center);
	setTextPositions(position, center);
}

void InformationPanel::setTextPositions(sf::Vector2f position, bool center)
{
	assert(texts.size() > 0);

	if(center)
	{
		sf::FloatRect background = getBackground().getGlobalBounds();
		sf::Vector2f centerOffset = { -background.width / 2, -background.height / 2 };
		position.x += centerOffset.x;
		position.y += centerOffset.y;
	}

	const int x = position.x + borderPadding;  // x pos of all texts.
	const int firstY = position.y + borderPadding;  // y pos of first text.
	// Set the position of the first text.
	texts[0].setPosition(x, firstY);

	// Set the position of the remaining text.
	for(int i = 1; i < texts.size(); ++i)
	{
		const int y = texts[i - 1].getGlobalBounds().top + texts[i - 1].getGlobalBounds().height + textPadding;
		sf::Vector2f newPos(x, y);
		texts[i].setPosition(newPos);
	}
}
