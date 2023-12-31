#pragma once
#include "UIPanel.h"
#include "RichText.h"
#include <vector>

/* 
Simple UI panel that displays texts line by line.
*/
class InformationPanel : public UIPanel
{
public:
	// Standardised information panel.
	InformationPanel(std::vector<sfe::RichText> texts);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;

private:
	void setTextPositions(sf::Vector2f position, bool center);

	std::vector<sfe::RichText> texts;
	const float borderPadding = 6;
	const float textPadding = -2;
};

