#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include "LandTerritoryOccupancy.h"
#include "NavalTerritory.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, Grid graphics, NavalTerritory *navalTerritory)
	: Territory(id, graphics, std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land)
{
	// Create the port.
	if(navalTerritory != nullptr)
	{
		port = std::make_unique<Port>(*this, *navalTerritory);
	}
}

LandTerritory::LandTerritory(int id, Grid graphics)
	: LandTerritory(id, graphics, nullptr)
{
}

LandTerritory::LandTerritory(int id)
	: Territory(id, createRandomLandColor(), std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land)
{
}

void LandTerritory::saveToFile(std::ofstream &file) const
{
	Territory::saveToFile(file);
	if(port != nullptr)
	{
		// Save the territory ID of the associated naval territory with the port.
		file << "# port naval id" << std::endl;
		file << port.get()->getNavalTerritoryID();
	}
}

std::unique_ptr<Port>& LandTerritory::getPort()
{
	return port;
}

void LandTerritory::drawPort(sf::RenderWindow & window) const
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
	}
}

void LandTerritory::setTerrain(Terrain terrain)
{
	features.terrain = terrain;
	if(getDrawMode() == TerritoryDrawMode::terrain)
	{
		getGrid().setColor(features.terrain.color);
	}
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
