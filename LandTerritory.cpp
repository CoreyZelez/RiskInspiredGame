#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include "LandTerritoryOccupancy.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, Grid graphics)
	: Territory(id, graphics, std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land), landDistanceMap(*this)
{
}

LandTerritory::LandTerritory(int id)
	: Territory(id, createRandomLandColor(), std::make_unique<LandTerritoryOccupancy>(*this), TerritoryType::land), landDistanceMap(*this)
{
}

void LandTerritory::calculateDistances(const std::vector<Territory*>& territories)
{
	Territory::calculateDistances(territories);

	// Calculate the distances between adjacent landed territories only.
	std::vector<Territory*> landTerritories;
	// Construct landed territories from territories.
	for(const auto& territory : territories)
	{
		if(auto* landedTerritory = dynamic_cast<LandTerritory*>(territory))
		{
			landTerritories.push_back(landedTerritory);
		}
	}
	landDistanceMap.calculateDistances(landTerritories);
}

void LandTerritory::setIsCoastal(bool isCoastal)
{
	this->isCoastal = isCoastal;
}

bool LandTerritory::getIsCoastal() const
{
	return isCoastal;
}

std::string LandTerritory::getSaveLabel() const
{
	return landSaveLabel;
}
