#include "EstateManager.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "Maridom.h"
#include "Barony.h"
#include "Estate.h"
#include <fstream>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <random>

EstateManager::EstateManager(const GameplaySettings &gameplaySettings)
	: gameplaySettings(gameplaySettings)
{
}

void EstateManager::draw(sf::RenderWindow &window, Title title) const
{
	// Draw all estates. 
	for(const auto &element : estates)
	{
		for(const auto &estate : element.second)
		{
			estate.get()->draw(window);
		}
	}

	// Draw estates of given title.
	for(const auto &element : estates)
	{
		if(element.first != title)
		{
			continue;
		}
		for(const auto &estate : element.second)
		{
			estate.get()->draw(window);
		}
	}
}

void EstateManager::draw(sf::RenderWindow &window) const
{
	// Following code problematic if more titles equal in prominence to Title::barony are added.
	assert(static_cast<int>(Title::barony) == 1);

	for(const auto &element : estates)
	{
		for(const auto &estate : element.second)
		{
			estate.get()->draw(window);
		}
	}
}

void EstateManager::drawUnownedMaridoms(sf::RenderWindow &window) const
{
	// Determine if there are any maridoms that can be drawn.
	auto it = estates.find(Title::maridom);
	if(it == estates.end())
	{
		return;
	}

	for(const auto &maridom : estates.at(Title::maridom))
	{
		if(!maridom.get()->hasRuler())
		{
			maridom->draw(window);
		}
	}
}

void EstateManager::load(std::string mapName, std::vector<std::unique_ptr<LandTerritory>>& landTerritories, 
	std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories)
{
	std::ifstream file("res/maps/" + mapName + "/" + mapName + "_estates.txt");
	std::string line;

	while(std::getline(file, line))
	{
		if(line.size() == 0)  // Line is blank.
		{
			continue;
		}
		else if(line.compare(estateSaveLabel) == 0)
		{
			loadEstate(file);
		}
		else if(line.compare(baronySaveLabel) == 0)
		{
			loadBarony(file, landTerritories);
		}
		else if(line.compare(maridomSaveLabel) == 0)
		{
			loadMaridom(file, navalTerritories);
		}
	}
}

void EstateManager::loadBarony(std::ifstream &file, std::vector<std::unique_ptr<LandTerritory>>& landTerritories)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	assert(static_cast<Title>(std::stoi(line)) == Title::barony);
	Title title = static_cast<Title>(std::stoi(line));

	// Load barony name.
	std::string name = loadName(file);

	// Load barony color.
	sf::Color color = loadColor(file);

	// Load names of subfiefs of barony.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	LandTerritory *territory = nullptr;
	for(auto &landTerritory : landTerritories)
	{
		// Territory already allocated to a landed estate.
		if(landTerritory.get()->getId() == territoryID)
		{
			assert(allocatedTerritoryIDs.count(territoryID) == 0);
			allocatedTerritoryIDs.insert(territoryID);  // Track territory id so reconcileBaronies does not create new barony with same territory.
			territory = landTerritory.get();
			break;
		}
	}

	// Territory was removed so barony no longer exists.
	if(territory == nullptr)
	{
		return;
	}

	// Load the land army yield.
	const double landArmyYield = territory->getFeatures().calculateArmyYield();

	// Load the naval fleet yield.
	const double navalFleetYield = territory->getFeatures().calculateFleetYield();

	// Create the barony.
	std::unique_ptr<Estate> barony = std::make_unique<Barony>(gameplaySettings, *territory, color);
	barony.get()->initName(name);

	// Add the subfiefs.
	for(std::string &subfiefName : subfiefNames)
	{
		barony->addSubfief(getFief(subfiefName));
	}

	// Update grid otherwise won't have color.
	barony.get()->getGrid().update();

	// Add the barony to estates.
	estates[Title::barony].emplace_back(std::move(barony));
}

void EstateManager::loadMaridom(std::ifstream & file, std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	assert(static_cast<Title>(std::stoi(line)) == Title::maridom);
	Title title = static_cast<Title>(std::stoi(line));

	// Load maridom name.
	std::string name = loadName(file);

	// Load maridom color.
	sf::Color color = loadColor(file);

	// Load names of subfiefs of maridom.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	NavalTerritory *territory = nullptr;
	for(auto &navalTerritory : navalTerritories)
	{
		// Territory already allocated to a landed estate.
		if(navalTerritory.get()->getId() == territoryID)
		{
			assert(allocatedTerritoryIDs.count(territoryID) == 0);
			allocatedTerritoryIDs.insert(territoryID);  // Track territory id so reconcileBaronies does not create new barony with same territory.
			territory = navalTerritory.get();
			break;
		}
	}

	// Territory was removed so barony no longer exists.
	if(territory == nullptr)
	{
		return;
	}

	// Create the maridom.
	std::unique_ptr<Estate> maridom = std::make_unique<Maridom>(*territory);
	maridom.get()->initName(name);

	// Add the subfiefs.
	for(std::string &subfiefName : subfiefNames)
	{
		maridom->addSubfief(getFief(subfiefName));
	}

	// Update grid otherwise won't have color.
	maridom.get()->getGrid().update();

	estates[Title::maridom].emplace_back(std::move(maridom));
}

void EstateManager::loadEstate(std::ifstream &file)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	Title title = static_cast<Title>(std::stoi(line));

	// Load name.
	std::string name = loadName(file);

	// Load color.
	sf::Color color = loadColor(file);

	// Load the names of the subfiefs.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Create the estate.
	estates[title].emplace_back(std::make_unique<Estate>(title, color));
	Estate& estate = *estates[title].back().get();

	estate.initName(name);

	// Add the subfiefs.
	bool hasSubfief = false;
	for(std::string &subfiefName : subfiefNames)
	{
		Estate *subfief = getFief(subfiefName);

		// Subfief no longer exists.
		if(subfief == nullptr)
		{
			continue;
		}

		estate.addSubfief(subfief);
		hasSubfief = true;
	}

	// Only adds the estate to estates if it has a subfief.
	if(!hasSubfief)
	{
		estates[title].pop_back();
		return;
	}

	// Update grid otherwise won't have color.
	estate.getGrid().update();
}

std::string EstateManager::loadName(std::ifstream & file)
{
	std::string line;

	// Load name.
	std::getline(file, line);
	assert(line.compare("# name") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	std::string name = line;
	assert(allocatedEstateNames.count(name) == 0);
	// Ensure name cannot be reused.
	allocatedEstateNames.insert(name);
	return name;
}

sf::Color EstateManager::loadColor(std::ifstream & file)
{
	std::string line;

	std::getline(file, line);
	assert(line.compare("# color") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	std::istringstream iss(line);
	int r, g, b;
	iss >> r >> g >> b;
	return sf::Color(r, g, b);
}

std::vector<std::unique_ptr<Estate>>& EstateManager::getBaronies()
{
	// assert(estates.count(Title::barony) != 0);  // THIS ASSERT IS NOT VALID FOR UNIT TESTS!
	return estates[Title::barony];
}

Estate* EstateManager::getEstate(sf::Vector2f position, Title title, bool allowParent)
{
	for(auto &estate : estates[title])
	{
		if((allowParent || !estate.get()->hasParent()) && estate.get()->getGrid().containsPosition(position))
		{
			return estate.get();
		}
	}

	return nullptr;
}

const Estate* EstateManager::getEstate(sf::Vector2f position, Title title)  const
{
	for(auto it = estates.find(title); it != estates.end(); ++it)
	{
		const std::vector<std::unique_ptr<Estate>> &estateVector = it->second;
		for(const auto &estate : estateVector)
		{
			assert(estate->containsPosition(position) == estate->getGrid().containsPosition(position));
			if(estate->getTitle() == title && estate->containsPosition(position))
			{
				return estate.get();
			}
		}
	}

	return nullptr;
}

Estate* EstateManager::getFief(std::string name)
{
	for(auto title : getOrderedTitles())
	{
		for(auto &estate : estates[title])
		{
			if(estate.get()->getName().compare(name) == 0)
			{
				return estate.get();
			}
		}
	}
	// Missing fief.
	assert(false);
	return nullptr;
}

std::vector<std::string> EstateManager::loadSubfiefNames(std::ifstream &file)
{
	std::vector<std::string> names;
	std::string line;

	std::getline(file, line);
	assert(line.compare("# subfiefs") == 0);

	while(std::getline(file, line) && line[0] != '#' && line.size() > 0)
	{
		names.push_back(line);
	}

	return names;
}
