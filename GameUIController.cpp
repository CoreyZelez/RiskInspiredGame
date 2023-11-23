#include "GameUIController.h"
#include "InputUtility.h"
#include "Game.h"
#include "StrengthSlider.h"
#include <iostream>

GameUIController::GameUIController(Game &game, GameUI &gameUI)
	: game(game), gameUI(gameUI)
{
}

void GameUIController::handleInput(const sf::RenderWindow &window, sf::View &UIView)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	if(inputUtility.getButtonDown(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
		gameUI.notifyButtonDown(sf::Mouse::Left, worldPos);
	}
}

void GameUIController::handleUICreation(const sf::RenderWindow &window)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Display information about map estates.
	if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		const MapMode mapMode = game.getMapMode();
		switch(mapMode)
		{
		case MapMode::realm:
		{
			const Realm *realm = game.getRealm(worldPos);
			if(realm != nullptr)
			{
				gameUI.setLeftUI(realm->getUI(UIType::information));
			}
			break;
		}
		case MapMode::barony:
		{
			const Estate *estate = game.getEstate(worldPos, Title::baron);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::county:
		{
			const Estate *estate = game.getEstate(worldPos, Title::count);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::duchy:
		{
			const Estate *estate = game.getEstate(worldPos, Title::duke);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::kingdom:
		{
			const Estate *estate = game.getEstate(worldPos, Title::king);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::empire:
		{
			const Estate *estate = game.getEstate(worldPos, Title::emperor);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		}
	}

	// Set bottom UI component of strength slider.
	if(game.getState() == GameState::militarySelected && gameUI.noBottomUI())
	{
		std::unique_ptr<UIEntity> strengthSlider = std::make_unique<StrengthSlider>(game.getSelectedMilitary(), game.getSelectedStrength());
		gameUI.setBottomUI(std::move(strengthSlider));
	}
	else if(game.getState() != GameState::militarySelected)
	{
		gameUI.clearBottomUI();
	}
}
