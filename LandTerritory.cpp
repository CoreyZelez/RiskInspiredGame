#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Utility.h"
#include "LandTerritoryOccupancy.h"
#include <assert.h>
#include <iostream>
#include <fstream>

LandTerritory::LandTerritory(int id, Grid graphics)
	: Territory(id, graphics, std::make_unique<LandTerritoryOccupancy>(*this))
{
}

LandTerritory::LandTerritory(int id)
	: Territory(id, createRandomLandColor(), std::make_unique<LandTerritoryOccupancy>(*this))
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
	landTerritoryDistances = calculateDistancesBFS(landTerritories, true);
}

int LandTerritory::getDistance(const Territory &territory, bool sameType) const
{
	// Check path through any passable territory type.
	if(sameType == false)
	{
		return Territory::getDistance(territory, sameType);
	}

	// Check path through same territory type.
	// No connecting path exists.
	if(landTerritoryDistances.count(&territory) == 0)
	{
		return -1;
	}
	return landTerritoryDistances[&territory];
}

const std::set<Territory*>& LandTerritory::getAdjacencies(bool sameType) const
{
	if(sameType == false)
	{
		return Territory::getAdjacencies(sameType);
	}

	return landTerritoryAdjacencies;
}

std::set<Territory*>& LandTerritory::getAdjacencies(bool sameType)
{
	if(sameType == false)
	{
		return Territory::getAdjacencies(sameType);
	}

	return landTerritoryAdjacencies;
}

void LandTerritory::addAdjacencies(std::vector<Territory*>& territories)
{
	Territory::addAdjacencies(territories);
	// Add land territory adjacencies.
	std::set<Territory*> adjacencies = Territory::getAdjacencies(false);
	for(const auto& territory : adjacencies)
	{
		if(auto* landTerritory = dynamic_cast<LandTerritory*>(territory))
		{
			landTerritoryAdjacencies.insert(landTerritory);
		}
	}
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
