#include "TerritoryMaker.h"
#include "Map.h"
#include "Territory.h"
#include "InputUtility.h"
#include "ProsperityFactory.h"
#include "TerrainFactory.h"
#include "CultureFactory.h"
#include <assert.h>
#include <iostream>
#include <thread>

TerritoryMaker::TerritoryMaker(TerritoryManager &territoryManager)
	: territoryManager(territoryManager), claimedPositions(50000), 
	terrainEditor(adjustLandTerrain, territoryManager, TerrainFactory()), cultureEditor(adjustLandCulture, territoryManager, CultureFactory()), 
	prosperityEditor(adjustLandProsperities, territoryManager, ProsperityFactory())
{
	initClaimedPositions();
}

void TerritoryMaker::draw(sf::RenderWindow &window) const
{
	// fixedTerritoryVertices does not include the territory vertices of the selected territory.

	if(state == TerritoryMakerState::none || state == TerritoryMakerState::editTerritoryGrid)
	{
		territoryManager.draw(window);
		return;
		window.draw(fixedTerritoryVertices);
		if(selectedTerritory != nullptr)
		{
			selectedTerritory->getGrid().draw(window);
		}
	}
	else
	{
		territoryManager.draw(window);
	}

	territoryManager.drawPorts(window);
}

void TerritoryMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForView(view);

	handleInputForStateChange();
	
	handleInputForTerritorySelection(window);
	handleInputForTerritoryCreation();
	handleInputForTerritoryGridEdits(window);

	switch(state)
	{
	case TerritoryMakerState::editCulture:
		cultureEditor.handleInput(window);
		break;
	case TerritoryMakerState::editProsperities:
		prosperityEditor.handleInput(window);
		break;
	case TerritoryMakerState::editTerrain:
		terrainEditor.handleInput(window);
		break;
	}

	handleInputForPortCreation(window);

	inputClock.restart();
}

void TerritoryMaker::changeState(TerritoryMakerState state)
{
	this->state = state;

	if(state == TerritoryMakerState::editTerrain)
	{
		territoryManager.setDrawMode(TerritoryDrawMode::terrain);
	}
	else if(state == TerritoryMakerState::editCulture)
	{
		territoryManager.setDrawMode(TerritoryDrawMode::culture);
	}
	else if(state == TerritoryMakerState::editProsperities)
	{
		territoryManager.setDrawMode(TerritoryDrawMode::prosperity);
	}
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

void TerritoryMaker::handleInputForStateChange()
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Deselect territory if any selected and return to neutral state.
	if(inputUtility.getKeyPressed(sf::Keyboard::Enter))
	{
		selectedTerritory = nullptr;
		changeState(TerritoryMakerState::none);
		updateFixedTerritoriesVertices();
	}

	// Cannot change state until selected territory is saved.
	if(selectedTerritory != nullptr)
	{
		return;
	}

	// Enter terrain editor.
	if(inputUtility.getKeyPressed(sf::Keyboard::Z))
	{
		changeState(TerritoryMakerState::editTerrain);
	}

	// Enter core prosperity editor.
	if(inputUtility.getKeyPressed(sf::Keyboard::X))
	{
		changeState(TerritoryMakerState::editProsperities);
	}

	// Enter culture editor.
	if(inputUtility.getKeyPressed(sf::Keyboard::C))
	{
		changeState(TerritoryMakerState::editCulture);
	}

	// Port creation.
	if(inputUtility.getKeyPressed(sf::Keyboard::P))
	{
		if(state == TerritoryMakerState::none)
		{
			changeState(TerritoryMakerState::createPort);
			portTerritories = { nullptr, nullptr };
		}
	}
}

void TerritoryMaker::handleInputForPortCreation(const sf::RenderWindow &window)
{
	if(state != TerritoryMakerState::createPort)
	{
		return;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

	InputUtility &inputUtility = InputUtility::getInstance();
	if(inputUtility.getButtonPressed(sf::Mouse::Right))
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

void TerritoryMaker::handleInputForTerritorySelection(const sf::RenderWindow &window)
{
	if(state != TerritoryMakerState::none)
	{
		return;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

	InputUtility &inputUtility = InputUtility::getInstance();

	if(inputUtility.getButtonDown(sf::Mouse::Right))
	{
		// Select territory for modification.
		selectedTerritory = territoryManager.getLandTerritory(worldPos);
		if(selectedTerritory == nullptr)
		{
			selectedTerritory = territoryManager.getNavalTerritory(worldPos);
		}
		if(selectedTerritory != nullptr)
		{
			state = TerritoryMakerState::editTerritoryGrid;
			updateFixedTerritoriesVertices();
		}
	}
}

void TerritoryMaker::handleInputForTerritoryCreation()
{
	if(state != TerritoryMakerState::none)
	{
		return;
	}

	InputUtility &inputUtility = InputUtility::getInstance();

	if(inputUtility.getKeyPressed(sf::Keyboard::L))
	{
		// Create a new territory.
		territoryManager.removeEmptyTerritories();
		selectedTerritory = territoryManager.createLandTerritory();
		state = TerritoryMakerState::editTerritoryGrid;
		updateFixedTerritoriesVertices();
	}

	// Create naval territory.
	if(inputUtility.getKeyPressed(sf::Keyboard::N))
	{
		territoryManager.removeEmptyTerritories();
		selectedTerritory = territoryManager.createNavalTerritory();
		state = TerritoryMakerState::editTerritoryGrid;
		updateFixedTerritoriesVertices();
	}
}

void TerritoryMaker::handleInputForTerritoryGridEdits(const sf::RenderWindow &window)
{
	if(state != TerritoryMakerState::editTerritoryGrid)
	{
		return;
	}

	InputUtility &inputUtility = InputUtility::getInstance();

	// Remove territory square at mouse position.
	if(inputUtility.getButtonDown(sf::Mouse::Right))
	{
		removePosition(window);
	}
	else if(inputUtility.getButtonDown(sf::Mouse::Left))
	{
		addPosition(window);
	}

	if(inputUtility.getKeyPressed(sf::Keyboard::B) || inputUtility.getButtonPressed(sf::Mouse::Button::XButton1))
	{
		progressBrushSize();
	}
}

void TerritoryMaker::initClaimedPositions()
{
	const std::vector<const Territory*> territories = territoryManager.getTerritories();
	for(const Territory *territory : territories)
	{
		std::unordered_set<sf::Vector2i, Vector2iHash> positions = territory->getGrid().getPositions();
		claimedPositions.insert(positions.begin(), positions.end());
	}
}

void TerritoryMaker::updateFixedTerritoriesVertices()
{
	fixedTerritoryVertices.clear();
	fixedTerritoryVertices.setPrimitiveType(sf::Triangles);
	for(const Territory* territory : territoryManager.getTerritories())
	{
		// Add vertices to fixed vertices if territory not currently chosen for modification.
		if(territory != selectedTerritory)
		{
			const sf::VertexArray &territoryVertices = territory->getGrid().getVertexArray();
			for(size_t i = 0; i < territoryVertices.getVertexCount(); ++i)
			{
				fixedTerritoryVertices.append(territoryVertices[i]);
			}
		}
	}
}

void TerritoryMaker::progressBrushSize()
{
	if(brushSize == 1)
	{
		brushSize = 3;
	}
	else if(brushSize == 3)
	{
		brushSize = 5;
	}
	else if(brushSize == 5)
	{
		brushSize = 9;
	}
	else
	{
		brushSize = 1;
	}
}

std::vector<sf::Vector2i> TerritoryMaker::determineBrushGridPositions(sf::Vector2i centerPosition) const
{
	assert(brushSize % 2 == 1);
	std::vector<sf::Vector2i> positions;
	for(int x = -brushSize / 2; x <= brushSize / 2; ++x)
	{
		for(int y = -brushSize / 2; y <= brushSize / 2; ++y)
		{
			sf::Vector2i position = centerPosition;
			position.x += x;
			position.y += y;
			positions.push_back(position);
		}
	}
	return positions;
}

void TerritoryMaker::addPosition(const sf::RenderWindow & window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
	sf::Vector2i mouseGridPosition = calculateGridCoordinates(worldPos);

	std::vector<sf::Vector2i> gridPositions = determineBrushGridPositions(mouseGridPosition);

	// Add territory square at grid positions derived from brush.

	for(sf::Vector2i position : gridPositions)
	{
		if(claimedPositions.count(position) == 0)
		{
			selectedTerritory->getGrid().addPosition(position, false);
			claimedPositions.insert(position);
		}
	}

	selectedTerritory->getGrid().update();
}

void TerritoryMaker::removePosition(const sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
	sf::Vector2i mouseGridPosition = calculateGridCoordinates(worldPos);

	std::vector<sf::Vector2i> gridPositions = determineBrushGridPositions(mouseGridPosition);

	for(sf::Vector2i position : gridPositions)
	{
		if(claimedPositions.count(position) == 1)
		{
			if(selectedTerritory->getGrid().containsPosition(position))
			{
				claimedPositions.erase(position);
			}
			selectedTerritory->getGrid().removePosition(position, false);
		}
	}

	selectedTerritory->getGrid().update();

	//// Remove square if any and update claimed positions if square is removed.
	//if(selectedTerritory->getGrid().removeSquare(worldPos))
	//{
	//	sf::Vector2i position = calculateGridCoordinates(worldPos);
	//	claimedPositions.erase(position);
	//}
}


