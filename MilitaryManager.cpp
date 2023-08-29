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

MilitaryForce * MilitaryManager::getMilitary(sf::Vector2f position)
{
	for(MilitaryForce *military : militaries)
	{
		if(military->containsPosition(position))
		{

		}
	}
	return nullptr;
}

void MilitaryManager::addLandArmy(std::unique_ptr<LandArmy> army)
{
	assert(army != nullptr);
	assert(army.get()->getStrength() > 0);
	militaries.emplace_back(army.get());
	armies.emplace_back(std::move(army));
}

void MilitaryManager::addNavalFleet(std::unique_ptr<NavalFleet> fleet)
{
	assert(fleet != nullptr);
	assert(fleet.get()->getStrength() > 0);
	militaries.emplace_back(fleet.get());
	navies.emplace_back(std::move(fleet));
}


