#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "ColorUtility.h"
#include "LandTerritoryOccupancy.h"
#include "NavalTerritory.h"
#include "TerrainFactory.h"
#include "CultureFactory.h"
#include "EditorTerritory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, EditorGrid graphics, LandTerritoryFeatures features, NavalTerritory *navalTerritory)
	: Territory(TerritoryType::land, id, graphics, std::make_unique<LandTerritoryOccupancy>(*this)), features(features)
{
	// Create the port.
	if(navalTerritory != nullptr)
	{
		port = std::make_unique<Port>(*this, *navalTerritory);
	}
}

LandTerritory::LandTerritory(int id, EditorGrid graphics, NavalTerritory *navalTerritory)
	: Territory(TerritoryType::land, id, graphics, std::make_unique<LandTerritoryOccupancy>(*this)), 
	features(nullptr)
{
	// Create the port.
	if(navalTerritory != nullptr)
	{
		port = std::make_unique<Port>(*this, *navalTerritory);
	}

	initDefaultFeatures();
}

LandTerritory::LandTerritory(int id, EditorGrid graphics)
	: LandTerritory(id, graphics, nullptr)
{
}

LandTerritory::LandTerritory(int id)
	: Territory(TerritoryType::land, id, generateRandomLandColor(), std::make_unique<LandTerritoryOccupancy>(*this)),
	features(nullptr)
{
	initDefaultFeatures();
}

void LandTerritory::saveToFile(std::ofstream &file) const
{
	Territory::saveToFile(file);

	// Save features.
	features.saveToFile(file);

	// Save port.
	if(port != nullptr)
	{
		// Save the territory ID of the associated naval territory with the port.
		file << "# port naval id" << std::endl;
		file << port.get()->getNavalTerritoryID() << std::endl;
	}
}

std::unique_ptr<Port>& LandTerritory::getPort()
{
	return port;
}

void LandTerritory::drawPort(sf::RenderWindow &window) const
{
	if(port != nullptr)
	{
		port.get()->draw(window);
	}
}

std::string LandTerritory::getSaveLabel() const
{
	return landSaveLabel;
}

void LandTerritory::setDrawMode(TerritoryDrawMode mode)
{
	Territory::setDrawMode(mode);

	switch(mode)
	{
	case TerritoryDrawMode::terrain:
		getGrid().setColor(features.terrain.color);
		break;
	case TerritoryDrawMode::culture:
		getGrid().setColor(features.culture.color);
		break;
	case TerritoryDrawMode::prosperity:
		// IMPLEMENT: SET COLOR BASED ON LEVEL OF PROSPERITY. 
		getGrid().setColor(determineProsperityColor(features.prosperity));
		break;
	}
}

const LandTerritoryFeatures& LandTerritory::getFeatures() const
{
	return features;
}

void LandTerritory::createPort(NavalTerritory &navalTerritory)
{
	// Return if grids do not share border.
	if(!getGrid().sharesBorder(navalTerritory.getGrid()))
	{
		return;
	}

	port = std::make_unique<Port>(*this, navalTerritory);
}

void LandTerritory::deletePort()
{
	port = nullptr;
}

bool LandTerritory::hasPort() const
{
	return port != nullptr;
}

const Terrain& LandTerritory::getTerrain() const
{
	return features.terrain;
}

void LandTerritory::initDefaultFeatures()
{
	features.prosperity = 100;
	features.coreProsperity = 100;

	features.terrain;
	features.culture; 
}

int loadPortNavalID(std::ifstream & file)
{
	std::string line;
	int id;
	std::getline(file, line);
	assert(line.compare("# port naval id") == 0);
	file >> id;
	std::getline(file, line);
	return id;
}

LandTerritoryFeatures loadLandTerritoryFeatures(std::ifstream & file, const GameplaySettings *gameplaySettings)
{
	LandTerritoryFeatures features(gameplaySettings);

	std::string line;

	// Prosperity.
	std::getline(file, line);
	assert(line.compare("# prosperity") == 0);
	std::getline(file, line);
	features.prosperity = std::stoi(line);

	// Core prosperity.
	std::getline(file, line);
	assert(line.compare("# core prosperity") == 0);
	std::getline(file, line);
	features.coreProsperity = std::stoi(line);

	// Terrain.
	std::getline(file, line);
	assert(line.compare("# terrain id") == 0);
	std::getline(file, line);
	int terrainID = std::stoi(line);
	TerrainFactory terrainFactory;
	assert(terrainFactory.has(terrainID));
	features.terrain = terrainFactory.create(terrainID);

	// Culture
	std::getline(file, line);
	assert(line.compare("# culture id") == 0);
	std::getline(file, line);
	int cultureID = std::stoi(line);
	CultureFactory cultureFactory;
	assert(cultureFactory.has(cultureID));
	features.culture = cultureFactory.create(cultureID);

	return features;
}

