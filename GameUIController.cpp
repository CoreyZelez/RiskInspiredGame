#include "GameUIController.h"
#include "InputUtility.h"
#include "Game.h"
#include <iostream>

GameUIController::GameUIController(Game &game, GameUI &gameUI)
	: game(game), gameUI(gameUI)
{
}

void GameUIController::handleInput(const sf::RenderWindow &window, sf::View &UIView)
{
	
}

void GameUIController::handlePanelCreation(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Display information about map estates.
	if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		const MapMode mapMode = game.getMapMode();
		switch(mapMode)
		{
		case MapMode::realm:
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
			const Realm *realm = game.getRealm(worldPos);
			if(realm != nullptr)
			{
				gameUI.setLeftUI(realm->getUI(UIType::information));
			}
		}
	}
}
