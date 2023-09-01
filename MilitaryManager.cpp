#include "MilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>
#include <iostream>

void MilitaryManager::draw(sf::RenderWindow & window) const
{
	for(auto &military : militaries)
	{
		if(military->getStrength() > 0)
		{
			military->draw(window);
		}
	}
}

MilitaryForce* MilitaryManager::getMilitary(sf::Vector2f position)
{
	for(MilitaryForce *military : militaries)
	{
		if(military->containsPosition(position) && military->getStrength() > 0)
		{
			return military;
		}
	}
	return nullptr;
}

void MilitaryManager::removeDeadMilitaries()
{
	// Remove dead land armies.
	auto iterL = armies.begin();
	while(iterL != armies.end())
	{
		if(iterL->get()->getStrength() > 0)
		{
			++iterL;
		}
		else
		{
			assert(iterL->get()->getStrength() == 0);
			assert(militaries.count(iterL->get()) == 1);
			militaries.erase(iterL->get());
			iterL = armies.erase(iterL);
		}
	}

	// Remove dead naval fleets.
	auto iterN = navies.begin();
	while(iterN != navies.end())
	{
		if(iterN->get()->getStrength() > 0)
		{
			++iterN;
		}
		else
		{
			assert(iterN->get()->getStrength() == 0);
			assert(militaries.count(iterN->get()) == 1);
			militaries.erase(iterN->get());
			iterN = navies.erase(iterN);
		}
	}
}

void MilitaryManager::addLandArmy(std::unique_ptr<LandArmy> army)
{
	assert(army != nullptr);
	assert(army.get()->getStrength() > 0);
	militaries.insert(army.get());
	armies.emplace_back(std::move(army));
}

void MilitaryManager::addNavalFleet(std::unique_ptr<NavalFleet> fleet)
{
	assert(fleet != nullptr);
	assert(fleet.get()->getStrength() > 0);
	militaries.insert(fleet.get());
	navies.emplace_back(std::move(fleet));
}


