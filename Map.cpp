#include "Map.h"
#include <fstream>
#include <filesystem>
#include <iostream>

void Map::drawTerritoryMap(sf::RenderWindow &window)
{
	territoryManager.draw(window);
}

void Map::save(std::string name) const
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
}

void Map::load(std::string name)
{
	territoryManager.load(name);
}

TerritoryManager& Map::getTerritoryManager()
{
	return territoryManager;
}

EstateManager& Map::getEstateManager()
{
	return estateManager;
}
