#pragma once
#include "IController.h"

class Player;

class HumanPlayerController : public IController
{
public:
	HumanPlayerController(Player &player);

	// Handles user input.
	virtual void handleInput(const sf::RenderWindow &window, sf::View &view) override;

	// Empty function. 
	virtual void action();

private:
	Player &player;
};

