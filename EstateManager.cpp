#include "EstateManager.h"
#include "LandTerritory.h"
#include "Barony.h"
#include <random>

void EstateManager::save(std::string mapName) const
{
}

void EstateManager::load(std::string mapName)
{
}

void EstateManager::generateBaronies(std::vector<std::unique_ptr<LandTerritory>> &landTerritories)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> armyYieldDist(0.4, 0.9); 
	std::uniform_real_distribution<double> fleetYieldDist(0.2, 0.5); 

	for(auto &territory : landTerritories)
	{
		std::unique_ptr<LandedEstate> barony;
		if(territory.get()->getIsCoastal())
		{
			barony = std::make_unique<Barony>(*territory.get(), armyYieldDist(mt), fleetYieldDist(mt));
		}
		else
		{
			barony = std::make_unique<Barony>(*territory.get(), armyYieldDist(mt));
		}
		landedEstates.emplace_back(std::move(barony));
	}
}
