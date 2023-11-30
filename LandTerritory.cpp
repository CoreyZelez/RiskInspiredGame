#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include "LandTerritoryOccupancy.h"
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

std::string LandTerritory::getSaveLabel() const
{
	return landSaveLabel;
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
