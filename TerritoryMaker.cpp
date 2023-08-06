#include "TerritoryMaker.h"
#include "Map.h"
#include "Territory.h"
#include "InputUtility.h"
#include <assert.h>
#include <iostream>

TerritoryMaker::TerritoryMaker(TerritoryManager &territoryManager)
	: territoryManager(territoryManager)
{
}

void TerritoryMaker::draw(sf::RenderWindow &window) const
{
	territoryManager.draw(window);
}

void TerritoryMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForView(view);

	if(inputUtility.getButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Remove territory square at mouse position.
		if(state == TerritoryMakerState::editTerritory)
		{
			territory->removeSquare(worldPos);
		}
		// Select territory for modification.
		else if(state == TerritoryMakerState::none)
		{
			territory = territoryManager.getLandTerritory(worldPos);
			if(territory == nullptr)
			{
				territory = territoryManager.getNavalTerritory(worldPos);
			}
			if(territory != nullptr)
			{
				state = TerritoryMakerState::editTerritory;
			}
		}
	}
	else if(inputUtility.getButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
		// Add territory square at mouse position.
		if(state == TerritoryMakerState::editTerritory && !territoryManager.positionClaimed(worldPos))
		{
			territory->addSquare(worldPos);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::L))
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createLandTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::N))
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createNavalTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Enter))
	{
		territory = nullptr;
		state = TerritoryMakerState::none;
	}

	inputClock.restart();
}

void TerritoryMaker::handleInputForView(sf::View &view) const
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
	const float zoom = 500.0f;
	const float effectiveZoom = zoom * inputClock.getElapsedTime().asSeconds();
	if(inputUtility.getMouseScrollDirection() == 1)
	{
		view.zoom(1 - effectiveZoom);
	}
	else if(inputUtility.getMouseScrollDirection() == -1)
	{
		view.zoom(1 + effectiveZoom);
	}
}


