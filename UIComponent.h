#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UIDecoration.h"  // Include UIDecoration header

class UIComponent 
{
public:
	UIComponent() = default;
	UIComponent(const sf::Vector2f& position, float padding);

	void addDecoration(UIDecoration &decoration);
	void setPosition(const sf::Vector2f& position);
	void draw(sf::RenderWindow& window) const;  // Mark draw as const

private:
	sf::RectangleShape background;
	std::vector<UIDecoration> decorations;
	float padding;
};