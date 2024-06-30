#include "TerritoryMaker.h"
#include "Map.h"
#include "InputUtility.h"
#include "ProsperityFactory.h"
#include "TerrainFactory.h"
#include "CultureFactory.h"
#include <assert.h>
#include <iostream>
#include <thread>

TerritoryMaker::TerritoryMaker()
	: claimedPositions(50000), terrainEditor(adjustLandTerrain, *this, TerrainFactory()), 
	cultureEditor(adjustLandCulture, *this, CultureFactory()),
	prosperityEditor(adjustLandProsperities, *this, ProsperityFactory())
{
}

void TerritoryMaker::load(std::string mapName)
{
	std::ifstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");
	std::string line;

	while (std::getline(file, line))
	{
		if (line.size() == 0)  // Line is blank.
		{
			continue;
		}
		else if (line.compare(landSaveLabel) == 0)
		{
			loadLandTerritory(file);
		}
		else if (line.compare(navalSaveLabel) == 0)
		{
			loadNavalTerritory(file);
		}
	}

	initClaimedPositions();
}

void TerritoryMaker::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_territories.txt");

	// Save naval before land territories because land depend on naval for port creation.
	for (const EditorTerritory& territory : territories)
	{
		if (territory.getType() == TerritoryType::land)
		{
			territory.saveToFile(file);
			file << std::endl;
		}
	}

	// Save land territories.
	for (const EditorTerritory& territory : territories)
	{
		if (territory.getType() == TerritoryType::naval)
		{
			territory.saveToFile(file);
			file << std::endl;
		}
	}

}

void TerritoryMaker::draw(sf::RenderWindow &window) const
{
	// fixedTerritoryVertices does not include the territory vertices of the selected territory.

	if(state == TerritoryMakerState::none || state == TerritoryMakerState::editTerritoryGrid)
	{
		for (const EditorTerritory& territory : territories)
		{
			territory.draw(window);
		}
		return;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//window.draw(fixedTerritoryVertices);
		//if(selectedTerritory != nullptr)
		//{
		//	selectedTerritory->getGrid().draw(window);
		//}
	}
	else
	{
		for (const EditorTerritory& territory : territories)
		{
			territory.draw(window);
		}
	}

	drawPorts(window);
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

std::deque<EditorTerritory>& TerritoryMaker::getTerritories()
{
	return territories;
}

void TerritoryMaker::loadLandTerritory(std::ifstream& file)
{
	EditorGrid grid = loadTerritoryGrid(file);

	int id = loadTerritoryID(file);
	nextID = std::max(id + 1, nextID);

	LandTerritoryFeatures features = loadLandTerritoryFeatures(file, nullptr);

	// Load naval territory associated with port if exists.
	EditorTerritory *portNavalTerritory = nullptr;
	if (file.peek() == '#')
	{
		int portNavalID = loadPortNavalID(file);
		portNavalTerritory = getTerritory(portNavalID);
	}

	territories.emplace_back(EditorTerritory(TerritoryType::land, id, grid, features));

	if (portNavalTerritory != nullptr)
	{
		territories.back().createPort(*portNavalTerritory);
	}
}

void TerritoryMaker::loadNavalTerritory(std::ifstream& file)
{
	EditorGrid grid = loadTerritoryGrid(file);

	int id = loadTerritoryID(file);
	nextID = std::max(id + 1, nextID);

	territories.emplace_back(EditorTerritory(TerritoryType::naval, id, grid));
}

void TerritoryMaker::changeState(TerritoryMakerState state)
{
	this->state = state;

	if(state == TerritoryMakerState::editTerrain)
	{
		setDrawMode(TerritoryDrawMode::terrain);
	}
	else if(state == TerritoryMakerState::editCulture)
	{
		setDrawMode(TerritoryDrawMode::culture);
	}
	else if(state == TerritoryMakerState::editProsperities)
	{
		setDrawMode(TerritoryDrawMode::prosperity);
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
		if (selectedTerritory != nullptr && selectedTerritory->getType() == TerritoryType::land)
		{
			changeState(TerritoryMakerState::createPort);
		}
	}
}

void TerritoryMaker::handleInputForPortCreation(const sf::RenderWindow &window)
{
	if(state != TerritoryMakerState::createPort)
	{
		return;
	}

	InputUtility& inputUtility = InputUtility::getInstance();

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

	if(inputUtility.getButtonPressed(sf::Mouse::Right))
	{
		EditorTerritory* navalTerritory = getTerritory(TerritoryType::naval, worldPos);
		if (navalTerritory != nullptr)
		{
			createPort(*navalTerritory);
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
		selectedTerritory = getTerritory(worldPos);
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
		removeEmptyTerritories();
		selectedTerritory = &createTerritory(TerritoryType::land);
		state = TerritoryMakerState::editTerritoryGrid;
		updateFixedTerritoriesVertices();
	}

	// Create naval territory.
	if(inputUtility.getKeyPressed(sf::Keyboard::N))
	{
		removeEmptyTerritories();
		selectedTerritory = &createTerritory(TerritoryType::naval);
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

void TerritoryMaker::drawPorts(sf::RenderWindow &window) const
{
	for (const EditorTerritory& territory : territories)
	{
		territory.drawPort(window);
	}
}

void TerritoryMaker::setDrawMode(TerritoryDrawMode mode)
{
	for (EditorTerritory& territory : territories)
	{
		territory.setDrawMode(mode);
	}
}

EditorTerritory* TerritoryMaker::getTerritory(int id)
{
	for (EditorTerritory& territory : territories)
	{
		if (territory.getID() == id)
		{
			return &territory;
		}
	}
}

void TerritoryMaker::initClaimedPositions()
{
	for(const EditorTerritory &territory : territories)
	{
		std::unordered_set<sf::Vector2i, Vector2iHash> positions = territory.getGrid().getPositions();
		claimedPositions.insert(positions.begin(), positions.end());
	}
}

void TerritoryMaker::updateFixedTerritoriesVertices()
{
	fixedTerritoryVertices.clear();
	fixedTerritoryVertices.setPrimitiveType(sf::Triangles);
	for(const EditorTerritory& territory : territories)
	{
		// Add vertices to fixed vertices if territory not currently chosen for modification.
		if(&territory != selectedTerritory)
		{
			const sf::VertexArray &territoryVertices = territory.getGrid().getVertexArray();
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
	else if(brushSize == 9)
	{
		brushSize = 21;
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

	// Remove square if any and update claimed positions if square is removed.
	if(selectedTerritory->getGrid().removeSquare(worldPos))
	{
		sf::Vector2i position = calculateGridCoordinates(worldPos);
		claimedPositions.erase(position);
	}
}

void TerritoryMaker::removeEmptyTerritories()
{
	for (auto territory = territories.begin(); territory != territories.end();)
	{
		if (territory->getGrid().isEmpty())
		{

		}
		else
		{
			++territory;
		}
	}
}

void TerritoryMaker::removeTerritory(EditorTerritory& territory)
{
	for (auto iter = territories.begin(); iter != territories.end(); ++iter)
	{
		if (&(*iter) == &territory)
		{
			territories.erase(iter);
			return;
		}
	}

	throw std::logic_error("Territory does not exist.");
}

EditorTerritory& TerritoryMaker::createTerritory(TerritoryType type)
{
	territories.push_back(EditorTerritory(type, nextID++));
	return territories.back();
}

void TerritoryMaker::createPort(EditorTerritory& navalTerritory)
{
	assert(navalTerritory.getType() == TerritoryType::naval);

	if (selectedTerritory->isAdjacent(navalTerritory))
	{
		selectedTerritory->createPort(navalTerritory);
	}
}

EditorTerritory* TerritoryMaker::getTerritory(TerritoryType type, sf::Vector2f position)
{
	for (EditorTerritory& territory : territories)
	{
		if (territory.getType() != type)
		{
			continue;
		}

		if (territory.getGrid().containsPosition(position))
		{
			return &territory;
		}
	}

	return nullptr;
}

EditorTerritory* TerritoryMaker::getTerritory(sf::Vector2f position)
{
	for (EditorTerritory& territory : territories)
	{
		if (territory.getGrid().containsPosition(position))
		{
			return &territory;
		}
	}

	return nullptr;
}
