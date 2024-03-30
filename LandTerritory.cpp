#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include "LandTerritoryOccupancy.h"
#include "NavalTerritory.h"
#include "TerrainFactory.h"
#include "CultureFactory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, Grid graphics, LandTerritoryFeatures features, NavalTerritory *navalTerritory)
	: Territory(id, graphics, std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land), features(features)
{
	// Create the port.
	if(navalTerritory != nullptr)
	{
		port = std::make_unique<Port>(*this, *navalTerritory);
	}
}

LandTerritory::LandTerritory(int id, Grid graphics, NavalTerritory *navalTerritory)
	: Territory(id, graphics, std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land), 
	features(nullptr)
{
	// Create the port.
	if(navalTerritory != nullptr)
	{
		port = std::make_unique<Port>(*this, *navalTerritory);
	}

	initDefaultFeatures();
}

LandTerritory::LandTerritory(int id, Grid graphics)
	: LandTerritory(id, graphics, nullptr)
{
}

LandTerritory::LandTerritory(int id)
	: Territory(id, createRandomLandColor(), std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land), 
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

void LandTerritory::setTerrain(Terrain terrain)
{
	features.terrain = terrain;
	if(getDrawMode() == TerritoryDrawMode::terrain)
	{
		getGrid().setColor(terrain.color);
	}
}

void LandTerritory::setCulture(Culture culture)
{
	features.culture = culture;
	if(getDrawMode() == TerritoryDrawMode::culture)
	{
		getGrid().setColor(culture.color);
	}
}

void LandTerritory::setProsperities(int prosperity)
{
	features.coreProsperity = prosperity;
	features.prosperity = prosperity;
	if(getDrawMode() == TerritoryDrawMode::prosperity)
	{
		getGrid().setColor(determineProsperityColor(prosperity));
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

void adjustLandTerrain(LandTerritory &landTerritory, const Terrain &terrain)
{
	landTerritory.setTerrain(terrain);
}

void adjustLandCulture(LandTerritory &landTerritory, const Culture &culture)
{
	landTerritory.setCulture(culture);
}

void adjustLandProsperities(LandTerritory &landTerritory, const int &prosperity)
{
	landTerritory.setProsperities(prosperity);
}
