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
	distributeArmyReinforcements();
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

void MilitaryManager::removeArmyReserves(double percentRemoved)
{
	armyReserves *= (1 - percentRemoved);
}

float MilitaryManager::getArmyReinforcementRate() const
{
	const float maxRate = 0.9;
	const int maxRateAttainmentValue = 10;
	float rate = maxRate * armies.size() / (float)maxRateAttainmentValue;
	return std::min(rate, maxRate);
}

void MilitaryManager::addArmyReinforcements(double amount)
{
	assert(amount >= 0);
	armyReinforcements += amount;
}

void MilitaryManager::addArmyReserves(double amount)
{
    assert(amount >= 0);
    armyReinforcements += amount;
}

float MilitaryManager::getFleetReinforcementRate() const
{
	const float maxRate = 0.9;
	const int maxRateAttainmentValue = 10;
	float rate = maxRate * fleets.size() / (float)maxRateAttainmentValue;
	return std::min(rate, maxRate);
}

void MilitaryManager::addFleetReinforcements(double amount)
{
	assert(amount >= 0);
	amount += amount;
}

void MilitaryManager::addFleetReserves(double amount)
{
	assert(amount >= 0);
	amount += amount;
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
	totalStrength += armyReserves;
	totalStrength += armyReinforcements;
	for(const auto &army : armies)
	{
		totalStrength += army.get()->getTotalStrength();
	}
	return totalStrength;
}

int MilitaryManager::getArmyReserves() const
{
	return armyReserves;
}

int MilitaryManager::getTotalFleetStrength() const
{
	int totalStrength = 0;
	totalStrength += fleetReserves;
	totalStrength += fleetReinforcements;
	for(const auto &fleet : fleets)
	{
		totalStrength += fleet.get()->getTotalStrength();
	}
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

int MilitaryManager::getTotalFleetStrength(int minStamina) const
{
	int totalStrength = 0;
	for(const auto &fleet : fleets)
	{
		totalStrength += fleet.get()->getStrength(minStamina);
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

void MilitaryManager::distributeArmyReinforcements()
{
	// Number of reinforcements available prior to allocation.
	const int totalReinforcementAmount = armyReinforcements;
	const int totalArmyStrength = getTotalArmyStrength();

	// Allocates reinforcements to armies.
	// There may be remaining reinforcement strength after this process.
	// It will be deferred to next turn.
	for(auto &army : armies)
	{
		// No armies should be dead as they should have been removed prior to calling this function.
		assert(army != nullptr);
		assert(army.get()->getTotalStrength() > 0);

		// Allocate reinforcement to armies proportionate to their strength.
		const float strengthRatio = (float)army.get()->getTotalStrength() / (float)totalArmyStrength;
		int reinforcementAmount = (float)totalReinforcementAmount * strengthRatio;  
		reinforcementAmount = std::min(reinforcementAmount, (int)armyReinforcements);
		armyReinforcements -= reinforcementAmount;
		assert(armyReinforcements >= 0);
		army.get()->increaseStrength(reinforcementAmount);
	}
}

void MilitaryManager::distributeFleetReinforcements()
{
	// Number of reinforcements available prior to allocation.
	const int totalReinforcementAmount = fleetReinforcements;
	const int totalFleetStrength = getTotalFleetStrength();

	// Allocates reinforcements to armies.
	// There may be remaining reinforcement strength after this process.
	// It will be deferred to next turn.
	for(auto &fleet : fleets)
	{
		// No armies should be dead as they should have been removed prior to calling this function.
		assert(fleet != nullptr);
		assert(fleet.get()->getTotalStrength() > 0);

		// Allocate reinforcement to armies proportionate to their strength.
		const float strengthRatio = (float)fleet.get()->getTotalStrength() / (float)totalFleetStrength;
		int reinforcementAmount = (float)totalReinforcementAmount * strengthRatio;
		reinforcementAmount = std::min(reinforcementAmount, (int)fleetReinforcements);
		fleetReinforcements -= reinforcementAmount;
		assert(fleetReinforcements >= 0);
		fleet.get()->increaseStrength(reinforcementAmount);
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


