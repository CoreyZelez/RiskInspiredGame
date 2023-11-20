#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class UIEntity;

class GameUI
{
public:
	void draw(sf::RenderWindow &window) const;

	void setLeftUI(std::unique_ptr<UIEntity> entity);
	void setRightUI(std::unique_ptr<UIEntity> entity);
	void setBottomUI(std::unique_ptr<UIEntity> entity);

private:
	std::unique_ptr<UIEntity> leftUI;
	std::unique_ptr<UIEntity> rightUI;
	std::unique_ptr<UIEntity> bottomUI;

};

