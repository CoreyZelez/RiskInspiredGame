#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class UIEntity;

class GameUI
{
public:
	GameUI(const sf::View &UIView);

	void draw(sf::RenderWindow &window) const;

	void notifyButtonDown(sf::Mouse::Button button, sf::Vector2f position);
	void setLeftUI(std::unique_ptr<UIEntity> entity);
	void setRightUI(std::unique_ptr<UIEntity> entity);
	void setBottomUI(std::unique_ptr<UIEntity> entity);
	bool noBottomUI();
	void clearBottomUI();

private:
	std::unique_ptr<UIEntity> leftUI;
	std::unique_ptr<UIEntity> rightUI;
	std::unique_ptr<UIEntity> bottomUI;
	const sf::View &UIView;

};

