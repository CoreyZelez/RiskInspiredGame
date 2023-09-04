#include "GameUI.h"
#include "IHasUI.h"
#include <iostream>

void GameUI::draw(sf::RenderWindow & window) const
{
	leftPanel.draw(window);
	bottomPanel.draw(window);
	rightPanel.draw(window);
}

void GameUI::setLeftPanel(const IHasUI &hasUI)
{
	leftPanel = hasUI.getUI();
	leftPanel.setPosition(sf::Vector2f(0, 0));
}

void GameUI::setRightPanel(const IHasUI &hasUI)
{
	rightPanel = hasUI.getUI();
}

void GameUI::setBottomPanel(const IHasUI & hasUI)
{
	bottomPanel = hasUI.getUI();
}
