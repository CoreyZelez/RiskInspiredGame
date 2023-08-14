#include "EstateManager.h"
#include "LandTerritory.h"
#include "Barony.h"
#include "Estate.h"
#include <random>
#include <assert.h>
#include <iostream>

void EstateManager::draw(sf::RenderWindow &window, Title title) const
{
	// Draw all estates. 
	for(const auto &element : estates)
	{
		for(const auto &estate : element.second)
		{
			estate.get()->drawGrid(window);
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
			estate.get()->drawGrid(window);
		}
	}
}

void EstateManager::draw(sf::RenderWindow &window) const
{
	// Following code problematic if more titles equal in prominence to Title::baron are added.
	assert(static_cast<int>(Title::baron) == 1);

	for(const auto &element : estates)
	{
		for(const auto &estate : element.second)
		{
			estate.get()->drawGrid(window);
		}
	}
}

void EstateManager::save(std::string mapName) const
{
}

void EstateManager::load(std::string mapName)
{
}

void EstateManager::reconcileBaronies(const std::vector<std::shared_ptr<LandTerritory>> &landTerritories)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> armyYieldDist(0.4, 0.9); 
	std::uniform_real_distribution<double> fleetYieldDist(0.2, 0.5); 

	for(auto &territory : landTerritories)
	{
		// Territory already allocated to a landed estate.
		if(allocatedIDs.count(territory.get()->getID()) == 1)
		{
			continue;
		}

		std::shared_ptr<LandedEstate> barony;
		if(territory.get()->getIsCoastal())
		{
			// TEMP. IN FUTURE ALLOW CUSTOMISATION OF BARONY PROSPERITY!!!
			barony = std::make_shared<Barony>(*territory.get(), armyYieldDist(mt), fleetYieldDist(mt));
		}
		else
		{
			// TEMP. IN FUTURE ALLOW CUSTOMISATION OF BARONY PROSPERITY!!!
			barony = std::make_shared<Barony>(*territory.get(), armyYieldDist(mt));
		}

		allocatedIDs.insert(territory.get()->getID());  // Ensures ID cannot be reused for other landed estate.
		landedEstates.emplace_back(barony);
		estates[Title::baron].emplace_back(barony);
	}
}

std::shared_ptr<Estate> EstateManager::createEstate(Title title)
{
	assert(title != Title::baron);
	std::shared_ptr<Estate> estate = std::make_shared<Estate>(title);
	estates[title].emplace_back(estate);
	return estate;
}

std::shared_ptr<Estate> EstateManager::getEstate(sf::Vector2f position, Title title, bool allowParent)
{
	for(auto &estate : estates[title])
	{
		if((allowParent || !estate.get()->hasParent()) && estate.get()->gridContainsPosition(position))
		{
			return estate;
		}
	}
	
	return nullptr;
}

std::shared_ptr<Estate> EstateManager::getLowerEstate(sf::Vector2f position, Title title, bool allowParent)
{
	// Following code problematic if more titles equal in prominence to Title::baron are added.
	assert(static_cast<int>(Title::baron) == 1);
	Title underTitle = title - 1;  // Title directly under parameter title.
	for(Title currTitle = underTitle; currTitle >= Title::baron; currTitle--)
	{
		for(auto &estate : estates[currTitle])
		{
			if((allowParent || !estate.get()->hasParent()) && estate.get()->gridContainsPosition(position))
			{
				return estate;
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

	setTitleColor(Title::baron, grey);
	setTitleColor(Title::count, grey);
	setTitleColor(Title::duke, grey);
	setTitleColor(Title::king, grey);
	setTitleColor(Title::emperor, grey);

	if(title == Title::baron || (Title::baron < title && setLower))
	{
		setTitleColor(Title::baron, baronyColor);
	}
	if(title == Title::count || (Title::count < title && setLower))
	{
		setTitleColor(Title::count, countyColor);

	}
	if(title == Title::duke || (Title::duke < title && setLower))
	{
		setTitleColor(Title::duke, dukedomColor);

	}
	if(title == Title::king || (Title::king < title && setLower))
	{
		setTitleColor(Title::king, kingdomColor);

	}
	if(title == Title::emperor || (Title::emperor < title && setLower))
	{
		setTitleColor(Title::emperor, empireColor);
	}
}

void EstateManager::setTitleColor(Title title, sf::Color color)
{
	for(auto &estate : estates[title])
	{
		estate.get()->setGridColor(color);
	}
}

