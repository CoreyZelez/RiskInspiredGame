#include "EditorTerritory.h"
#include "EditorEstate.h"
#include "EditorPort.h"
#include "ColorUtility.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <queue>

EditorTerritory::EditorTerritory(TerritoryType type, int id)
	: id(id), type(type)
{
	if (type == TerritoryType::land)
	{
		grid.setColor(generateRandomLandColor());
	}
	else if (type == TerritoryType::naval)
	{
		grid.setColor(generateRandomNavalColor());

	}
}

EditorTerritory::EditorTerritory(TerritoryType type, int id, const EditorGrid &grid)
	: id(id), type(type), grid(grid)
{
}

EditorTerritory::EditorTerritory(TerritoryType type, int id, const EditorGrid& grid, 
	LandTerritoryFeatures landTerritoryFeatures)
	: id(id), type(type), grid(grid), landTerritoryFeatures(landTerritoryFeatures)
{
}

void EditorTerritory::assignLandedEstate(EditorEstate* estate)
{
	assert(this->landedEstate == nullptr);
	assert(estate->getTitle() == Title::barony || estate->getTitle() == Title::maridom);
	landedEstate = estate;
}

void EditorTerritory::setDrawMode(TerritoryDrawMode mode)
{
	drawMode = mode;

	if (type == TerritoryType::land)
	{
		switch (mode)
		{
		case TerritoryDrawMode::terrain:
			getGrid().setColor(landTerritoryFeatures.terrain.color);
			break;
		case TerritoryDrawMode::culture:
			getGrid().setColor(landTerritoryFeatures.culture.color);
			break;
		case TerritoryDrawMode::prosperity:
			getGrid().setColor(determineProsperityColor(landTerritoryFeatures.prosperity));
			break;
		}
	}
}

bool EditorTerritory::isAdjacent(const EditorTerritory& territory) const
{
	return grid.sharesBorder(territory.grid);
}

void EditorTerritory::createPort(EditorTerritory& navalTerritory)
{
	assert(type == TerritoryType::land);
	assert(navalTerritory.getType() == TerritoryType::naval);
	assert(isAdjacent(navalTerritory));

	port = std::make_unique<EditorPort>(*this, navalTerritory);
}

TerritoryDrawMode EditorTerritory::getDrawMode() const
{
	return drawMode;
}

void EditorTerritory::saveToFile(std::ofstream& file) const
{
	assert(file.is_open());

	// Append data to the file.
	file << getSaveLabel() << std::endl;
	grid.saveToFile(file);
	file << "# id" << std::endl;
	file << id << std::endl;

	if (type == TerritoryType::land)
	{
		// Save features.
		landTerritoryFeatures.saveToFile(file);

		// Save port.
		if (port != nullptr)
		{
			// Save the territory ID of the associated naval territory with the port.
			file << "# port naval id" << std::endl;
			file << port.get()->getNavalTerritoryID() << std::endl;
		}
	}
}

void EditorTerritory::draw(sf::RenderWindow& window) const
{
	grid.draw(window);
}

void EditorTerritory::drawPort(sf::RenderWindow& window) const
{
	if (port != nullptr)
	{
		port.get()->draw(window);
	}
}

EditorGrid& EditorTerritory::getGrid()
{
	return grid;
}

const EditorGrid& EditorTerritory::getGrid() const
{
	return grid;
}

TerritoryType EditorTerritory::getType() const
{
	return type;
}

int EditorTerritory::getID() const
{
	return id;
}

std::string EditorTerritory::getSaveLabel() const
{
	if (type == TerritoryType::land)
	{
		return landSaveLabel;
	}
	else 
	{
		assert(type == TerritoryType::naval);
		return navalSaveLabel;
	}
}

LandTerritoryFeatures& EditorTerritory::getLandFeatures()
{
	assert(type == TerritoryType::land);
	return landTerritoryFeatures;
}

void adjustLandTerrain(EditorTerritory& landTerritory, const Terrain& terrain)
{
	assert(landTerritory.getType() == TerritoryType::land);

	LandTerritoryFeatures& features = landTerritory.getLandFeatures();

	features.terrain = terrain;
	if (landTerritory.getDrawMode() == TerritoryDrawMode::terrain)
	{
		landTerritory.getGrid().setColor(terrain.color);
	}
}

void adjustLandCulture(EditorTerritory& landTerritory, const Culture& culture)
{
	assert(landTerritory.getType() == TerritoryType::land);

	LandTerritoryFeatures& features = landTerritory.getLandFeatures();

	features.culture = culture;
	if (landTerritory.getDrawMode() == TerritoryDrawMode::culture)
	{
		landTerritory.getGrid().setColor(culture.color);
	}
}

void adjustLandProsperities(EditorTerritory& landTerritory, const int& prosperity)
{
	assert(landTerritory.getType() == TerritoryType::land);

	LandTerritoryFeatures& features = landTerritory.getLandFeatures();

	features.coreProsperity = prosperity;
	features.prosperity = prosperity;
	if (landTerritory.getDrawMode() == TerritoryDrawMode::prosperity)
	{
		landTerritory.getGrid().setColor(determineProsperityColor(prosperity));
	}
}