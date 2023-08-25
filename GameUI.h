#pragma once
#include <SFML/Graphics.hpp>

class Estate;

class GameUI
{
public:
	void draw(sf::RenderWindow &window) const;

	// Displays interaction panel for specified Estate.
	void loadEstatePanel(const Estate &estate);
};

