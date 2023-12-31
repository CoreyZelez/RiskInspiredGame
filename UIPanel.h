#pragma once
#include <SFML/Graphics.hpp>
#include "UIEntity.h"

class UIPanel : public UIEntity
{
public:
	UIPanel(sf::Color backgroundColor);
	virtual void draw(sf::RenderWindow &window) const;
	virtual void setPosition(sf::Vector2f position, bool center = false) override;

protected:
	const sf::RectangleShape& getBackground() const;
	void resizeBackground(float width, float height);

private:
	sf::RectangleShape background;
};

