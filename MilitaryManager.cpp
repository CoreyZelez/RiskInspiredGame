#include "MilitaryManager.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>
#include <iostream>

void MilitaryManager::draw(sf::RenderWindow & window) const
{
	for(auto &military : militaries)
	{
		if(!military->isDead())
		{
			military->draw(window);
		}
	}
}

void MilitaryManager::update()
{
	removeDeadMilitaries();
	resetStaminas();
}

MilitaryForce* MilitaryManager::getMilitary(sf::Vector2f position)
{
	for(MilitaryForce *military : militaries)
	{
		if(military->containsPosition(position) && !military->isDead())
		{
			return military;
		}
	}
	return nullptr;
}

LandArmy* MilitaryManager::getArmy(const Territory *territory)
{
	for(auto &army : armies)
	{
		if(&army.get()->getTerritory() == territory)
		{
			return army.get();
		}
	}
	return nullptr;
}

NavalFleet * MilitaryManager::getFleet(const Territory *territory)
{
	for(auto &fleet : fleets)
	{
		if(&fleet.get()->getTerritory() == territory)
		{
			return fleet.get();
		}
	}
	return nullptr;
}

const std::vector<std::unique_ptr<LandArmy>>& MilitaryManager::getArmies() const
{
	return armies;
}

std::vector<std::unique_ptr<LandArmy>>& MilitaryManager::getArmies()
{
	return armies;
}

const std::vector<std::unique_ptr<NavalFleet>>& MilitaryManager::getFleets() const
{
	return fleets;
}

std::vector<std::unique_ptr<NavalFleet>>& MilitaryManager::getFleets()
{
	return fleets;
}

int MilitaryManager::getTotalArmyStrength() const
{
	int totalStrength = 0;
	for(const auto &army : armies)
	{
		totalStrength += army.get()->getTotalStrength();
	}
	assert(totalStrength == getTotalArmyStrength(0));
	return totalStrength;
}

int MilitaryManager::getTotalArmyStrength(int minStamina) const
{
	int totalStrength = 0;
	for(const auto &army : armies)
	{
		totalStrength += army.get()->getStrength(minStamina);
	}
	return totalStrength;
}

void MilitaryManager::removeDeadMilitaries()
{
	// Remove dead land armies.
	auto iterL = armies.begin();
	while(iterL != armies.end())
	{
		if(!iterL->get()->isDead())
		{
			++iterL;
		}
		else
		{
			assert(iterL->get()->getTotalStrength() == 0);
			assert(militaries.count(iterL->get()) == 1);
			militaries.erase(iterL->get());
			iterL = armies.erase(iterL);
		}
	}

	// Remove dead naval fleets.
	auto iterN = fleets.begin();
	while(iterN != fleets.end())
	{
		if(!iterN->get()->isDead())
		{
			++iterN;
		}
		else
		{
			assert(iterN->get()->getTotalStrength() == 0);
			assert(militaries.count(iterN->get()) == 1);
			militaries.erase(iterN->get());
			iterN = fleets.erase(iterN);
		}
	}
}

void MilitaryManager::resetStaminas()
{
	for(auto &army : armies)
	{
		army.get()->resetStamina();
	}

	for(auto &fleet : fleets)
	{
		fleet.get()->resetStamina();
	}
}

void MilitaryManager::addLandArmy(std::unique_ptr<LandArmy> army)
{
	assert(army != nullptr);
	assert(army.get()->getTotalStrength() > 0);
	militaries.insert(army.get());
	armies.emplace_back(std::move(army));
}

void MilitaryManager::addNavalFleet(std::unique_ptr<NavalFleet> fleet)
{
	assert(fleet != nullptr);
	assert(fleet.get()->getTotalStrength() > 0);
	militaries.insert(fleet.get());
	fleets.emplace_back(std::move(fleet));
}


