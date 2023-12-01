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

		// Select territories for port creation.
		if(state == TerritoryMakerState::createPort)
		{
			assert(portTerritories.first == nullptr || portTerritories.second == nullptr);

			if(portTerritories.first == nullptr)
			{
				portTerritories.first = territoryManager.getLandTerritory(worldPos);
				if(portTerritories.first == nullptr)
				{
					portTerritories.second = nullptr;
					state = TerritoryMakerState::none;
				}
			}
			else if(portTerritories.second == nullptr)
			{
				portTerritories.second = territoryManager.getNavalTerritory(worldPos);
				if(portTerritories.second == nullptr)
				{
					portTerritories.first = nullptr;
					state = TerritoryMakerState::none;
				}
			}

			if(portTerritories.first != nullptr && portTerritories.second != nullptr)
			{
				// Create the port. Port is not created if territories not adjacent.
				portTerritories.first->createPort(*portTerritories.second);
				portTerritories.first = nullptr;
				portTerritories.second = nullptr;
				state = TerritoryMakerState::none;
			}

		}
	}
	if(inputUtility.getButtonDown(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Remove territory square at mouse position.
		if(state == TerritoryMakerState::editTerritory)
		{
			territory->getGrid().removeSquare(worldPos);
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
	if(inputUtility.getButtonDown(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
		// Add territory square at mouse position.
		if(state == TerritoryMakerState::editTerritory && !territoryManager.positionClaimed(worldPos))
		{
			territory->getGrid().addSquare(worldPos);
		}
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::L))
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createLandTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::N))
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createNavalTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::Enter))
	{
		territory = nullptr;
		state = TerritoryMakerState::none;
	}
	if(inputUtility.getKeyPressed(sf::Keyboard::P))
	{
		if(state == TerritoryMakerState::none)
		{
			std::cout << "LETS MAKE A PORT!" << std::endl;
			state = TerritoryMakerState::createPort;
			portTerritories = { nullptr, nullptr };
		}
	}

	inputClock.restart();
}

void TerritoryMaker::handleInputForView(sf::View &view) const
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle view movement.
	const float speed = 3000.0f;
	const float effectiveSpeed = speed * inputClock.getElapsedTime().asSeconds();
	if(inputUtility.getKeyDown(sf::Keyboard::W))
	{
		view.move(0, -effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::A))
	{
		view.move(-effectiveSpeed, 0);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::S))
	{
		view.move(0, effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::D))
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


