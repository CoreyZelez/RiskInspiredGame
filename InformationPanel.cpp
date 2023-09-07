#include "InformationPanel.h"
#include "FontManager.h"
#include <assert.h>
#include <iostream>

InformationPanel::InformationPanel(std::vector<sfe::RichText> texts)
	: UIPanel(sf::Color(50, 50, 50, 180)), texts(texts)
{
	float backgroundWidth = 0;
	float backgroundHeight = 0; 
	for(sfe::RichText &text : this->texts)
	{
		text.setCharacterSize(18);  // All texts have same font size.
	
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

void InformationPanel::handleInteraction(sf::Vector2f position)
{
}

void InformationPanel::setPosition(sf::Vector2f position)
{
	UIPanel::setPosition(position);
	setTextPositions(position);
}

void InformationPanel::setTextPositions(sf::Vector2f position)
{
	assert(texts.size() > 0);

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
