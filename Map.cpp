#include "Map.h"
#include <fstream>
#include <filesystem>
#include <iostream>

Map::Map(std::string name)
	: name(name)
{
	// Attempts to load map from file. If not in files then map is empty.
	load(name);
}

void Map::drawTerritoryMap(sf::RenderWindow &window)
{
	territoryManager.draw(window);
}

void Map::save() 
{
	saveAs(name);
}

void Map::saveAs(std::string name) 
{
	namespace fs = std::filesystem;

	const std::string folderPath = "res/maps/" + name;
	
	// Create the folder
	if(!fs::exists(folderPath))
	{
		if(!fs::create_directory(folderPath))
		{
			std::cerr << "Failed to create the folder." << std::endl;
			exit(1);
		}
	}

	territoryManager.save(name);
	estateManager.save(name);
	this->name = name;  
}

void Map::load(std::string name)
{
	territoryManager.load(name);
	estateManager.load(name, territoryManager.getLandTerritories());
}

TerritoryManager& Map::getTerritoryManager()
{
	return territoryManager;
}

EstateManager& Map::getEstateManager()
{
	return estateManager;
}
