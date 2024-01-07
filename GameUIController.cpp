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
		sf::Vector2f UIPos = window.mapPixelToCoords(mousePos, UIView);
		gameUI.notifyButtonDown(sf::Mouse::Left, UIPos);
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
			// Const version of game so we can call const getRealm function.
			const Game &constGame = game;
			const bool considerVassalView = true;
			const Realm *realm = constGame.getRealm(worldPos, considerVassalView);
			if(realm != nullptr)
			{
				gameUI.setLeftUI(realm->getUI(UIType::information));
			}
			break;
		}
		case MapMode::barony:
		{
			const Estate *estate = game.getEstate(worldPos, Title::barony);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::county:
		{
			const Estate *estate = game.getEstate(worldPos, Title::county);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::duchy:
		{
			const Estate *estate = game.getEstate(worldPos, Title::duchy);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::kingdom:
		{
			const Estate *estate = game.getEstate(worldPos, Title::kingdom);
			if(estate != nullptr)
			{
				gameUI.setLeftUI(estate->getUI(UIType::information));
			}
			break;
		}
		case MapMode::empire:
		{
			const Estate *estate = game.getEstate(worldPos, Title::empire);
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
