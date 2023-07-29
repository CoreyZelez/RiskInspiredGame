#include "TerritoryMaker.h"
#include "Map.h"
#include "Territory.h"
#include <assert.h>
#include <iostream>

TerritoryMaker::TerritoryMaker(TerritoryManager &territoryManager)
	: territoryManager(territoryManager)
{
}

void TerritoryMaker::draw(sf::RenderWindow &window)
{
	territoryManager.draw(window);
}

void TerritoryMaker::handleEvent(const sf::RenderWindow &window, const sf::Event &event)
{
	if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
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
	else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Add territory square at mouse position.
		if(state == TerritoryMakerState::editTerritory && !territoryManager.positionClaimed(worldPos))
		{
			territory->addSquare(worldPos);
		}
	}
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createLandTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		territory = territoryManager.createNavalTerritory();
		state = TerritoryMakerState::editTerritory;
	}
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
	{
		territory = nullptr;
		state = TerritoryMakerState::none;
	}

}
