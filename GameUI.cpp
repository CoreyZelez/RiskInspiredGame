#include "GameUI.h"
#include "UIPanel.h"
#include <iostream>

GameUI::GameUI(const sf::View & UIView)
	: UIView(UIView)
{
}

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

void GameUI::notifyButtonDown(sf::Mouse::Button button, sf::Vector2f position)
{
	if(leftUI != nullptr)
	{
		leftUI.get()->handleButtonDown(button, position);
	}
	if(rightUI != nullptr)
	{
		rightUI.get()->handleButtonDown(button, position);
	}
	if(bottomUI != nullptr)
	{
		bottomUI.get()->handleButtonDown(button, position);
	}
}

void GameUI::setLeftUI(std::unique_ptr<UIEntity> entity)
{
	float leftMostCoordinate = UIView.getCenter().x - UIView.getSize().x / 2.0f;
	leftUI = std::move(entity);
	leftUI.get()->setPosition(sf::Vector2f(leftMostCoordinate, 400));
}

void GameUI::resetLeftUI()
{
	leftUI = nullptr;
}

void GameUI::setRightUI(std::unique_ptr<UIEntity> entity)
{
	float leftMostCoordinate = UIView.getCenter().x - UIView.getSize().x / 2.0f;
	leftUI = std::move(entity);
	leftUI.get()->setPosition(sf::Vector2f(leftMostCoordinate, 400));
}

void GameUI::setBottomUI(std::unique_ptr<UIEntity> entity)
{
	bottomUI = std::move(entity);
	const double x = UIView.getCenter().x;
	const double y = 1200;
	bottomUI.get()->setPosition(sf::Vector2f(x, y), true);
}

void GameUI::clearBottomUI()
{
	bottomUI = nullptr;
}

bool GameUI::noBottomUI()
{
	return bottomUI.get() == nullptr;
}
