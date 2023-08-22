#pragma once
#include <SFML/Graphics.hpp>

class IController
{
public:
	// Handles user input.
	virtual void handleInput(const sf::RenderWindow &window, sf::View &view) = 0;

	// Performs an action.
	virtual void action() = 0;
};

