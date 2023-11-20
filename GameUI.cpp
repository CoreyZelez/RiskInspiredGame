#include "GameUI.h"
#include "UIPanel.h"
#include <iostream>

void GameUI::draw(sf::RenderWindow &window) const
{
	if(leftUI != nullptr)
	{
		leftUI.get()->draw(window);
	}
	if(rightUI != nullptr)
	{
		rightUI.get()->draw(window);
	}
	if(bottomUI != nullptr)
	{
		bottomUI.get()->draw(window);
	}
}

void GameUI::setLeftUI(std::unique_ptr<UIEntity> entity)
{
	leftUI = std::move(entity);
	leftUI.get()->setPosition(sf::Vector2f(0, 400));
}

void GameUI::setRightUI(std::unique_ptr<UIEntity> entity)
{
	rightUI = std::move(entity);
}

void GameUI::setBottomUI(std::unique_ptr<UIEntity> entity)
{
	bottomUI = std::move(entity);
}
