#include "GameUI.h"
#include "UIPanel.h"
#include <iostream>

void GameUI::draw(sf::RenderWindow &window) const
{
	if(leftPanel != nullptr)
	{
		leftPanel.get()->draw(window);
	}
	if(rightPanel != nullptr)
	{
		rightPanel.get()->draw(window);
	}
	if(bottomPanel != nullptr)
	{
		bottomPanel.get()->draw(window);
	}
}

void GameUI::setLeftPanel(std::unique_ptr<UIPanel> panel)
{
	leftPanel = std::move(panel);
	leftPanel.get()->setPosition(sf::Vector2f(0, 400));
}

void GameUI::setRightPanel(std::unique_ptr<UIPanel> panel)
{
	rightPanel = std::move(panel);
}

void GameUI::setBottomPanel(std::unique_ptr<UIPanel> panel)
{
	bottomPanel = std::move(panel);
}
