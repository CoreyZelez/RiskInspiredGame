#pragma once
#include <SFML/Graphics.hpp>
#include "UIEntity.h"

class UIPanel : public UIEntity
{
public:
	UIPanel(sf::Color backgroundColor);
	virtual void draw(sf::RenderWindow &window) const;
	virtual void handleLeftClick(sf::Vector2f position) = 0;
	virtual void setPosition(sf::Vector2f position);

protected:
	void resizeBackground(float width, float height);

private:
	sf::RectangleShape background;
};

