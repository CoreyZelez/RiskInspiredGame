#include "EstateMaker.h"
#include "InputUtility.h"
#include "EstateManager.h"
#include <iostream>

EstateMaker::EstateMaker(EstateManager &estateManager)
	: estateManager(estateManager)
{
}

void EstateMaker::draw(sf::RenderWindow &window) 
{
	if(state == EstateMakerState::none)
	{
		estateManager.draw(window);
	}
	else if(state == EstateMakerState::selectEstate)
	{
		estateManager.draw(window, selectedTitle);
	}
	else if(state == EstateMakerState::editEstate)
	{
		// MUST IMPLEMENT!
		///estateManager.draw(window, selectedTitle - 1);

		estateManager.draw(window, selectedTitle);
		estate->drawGrid(window);
	}
}

void EstateMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForView(view);

	if(inputUtility.getButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Select estate for modification.
		if(state == EstateMakerState::selectEstate)
		{
			// Selects estate with selectedTitle. Can have a parent estate.
			estate = estateManager.getEstate(worldPos, selectedTitle, true);
			changeState(EstateMakerState::editEstate);
		}
		// Removes subfiefs from estate.
		else if(state == EstateMakerState::editEstate)
		{
			Estate *subfief = estateManager.getLowerEstate(worldPos, selectedTitle, true);
			// Remove subFief if it is a sub fief of estate. 
			if(subfief != nullptr)
			{
				estate->removeSubfief(subfief);
			}
		}
	}
	else if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Add subfiefs to estate.
		if(state == EstateMakerState::editEstate)
		{
			// Adds parentless and lower title estate at mouse position as a subfief.
			Estate *subfief = estateManager.getLowerEstate(worldPos, selectedTitle, false);  
			if(subfief != nullptr)
			{
				assert(subfief->hasParent() == false);
				estate->addSubfief(subfief);
			}
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num2))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::count;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num3))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::duke;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num4))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::king;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num5))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::emperor;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::R))
	{
		if(state == EstateMakerState::selectEstate)
		{
			estate = estateManager.createEstate(selectedTitle);
			changeState(EstateMakerState::editEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Enter))
	{
		if(state == EstateMakerState::editEstate)
		{
			estate = nullptr;
			changeState(EstateMakerState::none);
		}
	}

	inputClock.restart();
}

void EstateMaker::changeState(EstateMakerState state)
{
	if(state == EstateMakerState::selectEstate)
	{
		estateManager.makeColored(selectedTitle, false);
	}
	else if(state == EstateMakerState::editEstate)
	{
		estateManager.makeColored(selectedTitle, true);
		const sf::Color selectedEstateColor(240, 240, 0);
		estate->setGridColor(selectedEstateColor);
	}

	this->state = state;
}

void EstateMaker::handleInputForView(sf::View &view) const
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle view movement.
	const float speed = 3000.0f;
	const float effectiveSpeed = speed * inputClock.getElapsedTime().asSeconds();
	if(inputUtility.getKeyPressed(sf::Keyboard::W))
	{
		view.move(0, -effectiveSpeed);
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::A))
	{
		view.move(-effectiveSpeed, 0);
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::S))
	{
		view.move(0, effectiveSpeed);
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::D))
	{
		view.move(effectiveSpeed, 0);
	}

	// Handle view zoom.
	const float zoom = 0.1f;
	if(inputUtility.getMouseScrollDirection() == 1)
	{
		view.zoom(1 - zoom);
	}
	else if(inputUtility.getMouseScrollDirection() == -1)
	{
		view.zoom(1 + zoom);
	}
}


