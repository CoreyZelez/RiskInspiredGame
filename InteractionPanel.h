#pragma once
#include "UIPanel.h"

/*
Simple UI panel containing other UI entities (not panels) for interacting with.
*/
class InteractionPanel : public UIPanel
{
public:
	InteractionPanel(sf::Color backgroundColor, std::vector<std::vector<std::unique_ptr<UIEntity>>> UIEntities);

	virtual void draw(sf::RenderWindow &window) const override;
	virtual void handleButtonDown(sf::Mouse::Button button, sf::Vector2f position) override;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;

private:
	// Holds UIEntities in position of row and column.
	std::vector<std::vector<std::unique_ptr<UIEntity>>> UIEntities;
	// Padding of background panel edges to UI entities.
	const float borderPadding = 40;
	// Padding of background panel edges to UI entities.
	const float horizontalEntityPadding = 30;
	// Padding of background panel edges to UI entities.
	const float verticalEntityPadding = 10;
};

