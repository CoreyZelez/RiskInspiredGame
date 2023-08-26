#include "MilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>
#include <iostream>

void MilitaryManager::draw(sf::RenderWindow & window) const
{
	for(auto &army : armies)
	{
		army.get()->draw(window);
	}
	
	for(auto &fleet : navies)
	{
		fleet.get()->draw(window);
	}
}

void MilitaryManager::addLandArmy(std::unique_ptr<LandArmy> army)
{
	assert(army != nullptr);
	assert(army.get()->getStrength() > 0);
	armies.emplace_back(std::move(army));
}

void MilitaryManager::addNavalFleet(std::unique_ptr<NavalFleet> fleet)
{
	assert(fleet != nullptr);
	assert(fleet.get()->getStrength() > 0);
	navies.emplace_back(std::move(fleet));
}


