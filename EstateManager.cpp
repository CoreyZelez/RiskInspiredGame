#include "EstateManager.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "Maridom.h"
#include "Barony.h"
#include "NameGenerator.h"
#include "Estate.h"
#include <fstream>
#include <random>
#include <assert.h>
#include <iostream>
#include <string>

EstateManager::EstateManager()
	: nameGenerator("estates")
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

void EstateManager::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_estates.txt");

	// Saves estates in order of title prestige.
	std::vector<Title> titles = getOrderedTitles();
	for(Title title : titles)
	{
		for(auto iter = estates.cbegin(); iter != estates.cend(); ++iter)
		{
			// Check that the key matches the title.
			if(iter->first != title)
			{
				continue;
			}
			// Save the estate.
			for(const auto &estate : iter->second)
			{
				estate.get()->saveToFile(file);
				file << std::endl;  // Gap in file signifys end of individual estate info.
			}
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

	// Set color of maridoms to greyish blue.
	setTitleColor(Title::maridom, sf::Color(90, 130, 255));
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

	// Load names of subfiefs of barony.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	LandTerritory *territory = nullptr;
	for(auto &landTerritory : landTerritories)
	{
		// Territory already allocated to a landed estate.
		if(landTerritory.get()->getID() == territoryID)
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
	std::getline(file, line);
	assert(line.compare("# land army yield") == 0);
	std::getline(file, line);
	const double landArmyYield = std::stod(line);

	// Load the naval fleet yield.
	std::getline(file, line);
	assert(line.compare("# naval fleet yield") == 0);
	std::getline(file, line);
	const double navalFleetYield = std::stod(line);

	// Create the barony.
	std::unique_ptr<Estate> barony = std::make_unique<Barony>(*territory, landArmyYield, navalFleetYield);
	barony.get()->initName(name);
	// Add the subfiefs.
	for(std::string &subfiefName : subfiefNames)
	{
		barony->addSubfief(getFief(subfiefName));
	}
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

	// Load names of subfiefs of maridom.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	NavalTerritory *territory = nullptr;
	for(auto &navalTerritory : navalTerritories)
	{
		// Territory already allocated to a landed estate.
		if(navalTerritory.get()->getID() == territoryID)
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
	// Add the barony to estates.
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

	// Load the names of the subfiefs.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Create the estate.
	std::unique_ptr<Estate> estate = std::make_unique<Estate>(title);
	estate.get()->initName(name);
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

		estate->addSubfief(subfief);
		hasSubfief = true;
	}
	// Only adds the estate to estates if it has a subfief.
	if(hasSubfief)
	{
		estates[title].emplace_back(std::move(estate));
	}
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

void EstateManager::reconcileBaronies(const std::vector<std::unique_ptr<LandTerritory>> &landTerritories)
{
	// TEMPORARY RNG FOR BARONIES!!!
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> armyYieldDist(0.4, 0.9);
	std::uniform_real_distribution<double> fleetYieldDist(0.2, 0.5);

	for(auto &territory : landTerritories)
	{
		// Territory already allocated to a landed estate.
		if(allocatedTerritoryIDs.count(territory.get()->getID()) == 1)
		{
			continue;
		}

		std::unique_ptr<Estate> barony = std::make_unique<Barony>(*territory.get(), armyYieldDist(mt), fleetYieldDist(mt));

		// Generate name for barony.
		barony.get()->initName(generateName());

		allocatedTerritoryIDs.insert(territory.get()->getID());  // Ensures ID cannot be reused for other landed estate.
		estates[Title::barony].emplace_back(std::move(barony));
	}
}

void EstateManager::reconcileMaridoms(const std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories)
{
	for(auto &territory : navalTerritories)
	{
		// Territory already allocated to a landed estate.
		if(allocatedTerritoryIDs.count(territory.get()->getID()) == 1)
		{
			continue;
		}

		std::unique_ptr<Estate> maridom = std::make_unique<Maridom>(*territory.get());

		// Generate name for maridom.
		maridom.get()->initName(generateName());

		allocatedTerritoryIDs.insert(territory.get()->getID());  // Ensures ID cannot be reused for other landed estate.
		estates[Title::maridom].emplace_back(std::move(maridom));
	}
}

std::vector<std::unique_ptr<Estate>>& EstateManager::getBaronies()
{
	// assert(estates.count(Title::barony) != 0);  // THIS ASSERT IS NOT VALID FOR UNIT TESTS!
	return estates[Title::barony];
}

Estate* EstateManager::createEstate(Title title)
{
	assert(title != Title::barony);

	std::unique_ptr<Estate> estate = std::make_unique<Estate>(title);
	estate.get()->initName(generateName());

	Estate *e = estate.get();
	estates[title].emplace_back(std::move(estate));
	return e;
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

Estate* EstateManager::getLowerEstate(sf::Vector2f position, Title title, bool allowParent)
{
	// Following code problematic if more titles equal in prominence to Title::barony are added.
	assert(static_cast<int>(Title::barony) == 1);
	Title underTitle = title - 1;  // Title directly under parameter title.
	for(Title currTitle = underTitle; currTitle >= Title::barony; currTitle--)
	{
		for(auto &estate : estates[currTitle])
		{
			if((allowParent || !estate.get()->hasParent()) && estate.get()->getGrid().containsPosition(position))
			{
				return estate.get();
			}
		}
	}
	return nullptr;
}

void EstateManager::makeColored(Title title, bool setLower)
{
	const sf::Color grey(100, 100, 100);
	const sf::Color baronyColor(189, 19, 0);
	const sf::Color countyColor(167, 218, 0);
	const sf::Color dukedomColor(41, 245, 0);
	const sf::Color kingdomColor(245, 179, 0);
	const sf::Color empireColor(245, 0, 0);

	setTitleColor(Title::barony, grey);
	setTitleColor(Title::county, grey);
	setTitleColor(Title::duchy, grey);
	setTitleColor(Title::kingdom, grey);
	setTitleColor(Title::empire, grey);

	if(title == Title::barony || (Title::barony < title && setLower))
	{
		setTitleColor(Title::barony, baronyColor);
	}
	if(title == Title::county || (Title::county < title && setLower))
	{
		setTitleColor(Title::county, countyColor);

	}
	if(title == Title::duchy || (Title::duchy < title && setLower))
	{
		setTitleColor(Title::duchy, dukedomColor);

	}
	if(title == Title::kingdom || (Title::kingdom < title && setLower))
	{
		setTitleColor(Title::kingdom, kingdomColor);

	}
	if(title == Title::empire || (Title::empire < title && setLower))
	{
		setTitleColor(Title::empire, empireColor);
	}
}

std::string EstateManager::generateName()
{
	std::string name;
	// LOOP POTENTIALLY NEVER TERMINATES IN PARTICULAR IF THERE IS NOT ENOUGH POSSIBLE NAMES.
	while(true)
	{
		name = nameGenerator.generateRandomName();
		if(allocatedEstateNames.count(name) == 0)
		{
			break;
		}
	}
	allocatedEstateNames.insert(name);
	return name;
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

void EstateManager::setTitleColor(Title title, sf::Color color)
{
	for(auto &estate : estates[title])
	{
		estate.get()->getGrid().setColor(color);
	}
}

