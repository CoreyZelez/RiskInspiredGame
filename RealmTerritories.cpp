#include "RealmTerritories.h"
#include "Territory.h"
#include "Player.h"
#include "RealmGrid.h"
#include <assert.h>

RealmTerritories::RealmTerritories(RealmGrid& grid)
	: grid(grid)
{
}

// All territories in realm territories are either controlled or sieged. Thus to remove all the territories,
// we need only check these two types.
void RealmTerritories::eraseTerritories(const RealmTerritories& territories)
{
	for(Territory* territory : territories.controlledTerritories)
	{
		eraseTerritory(*territory);
	}

	for(Territory* territory : territories.siegedTerritories)
	{
		eraseTerritory(*territory);
	}
}

void RealmTerritories::eraseTerritory(Territory& territory)
{
	controlledTerritories.erase(&territory);
	controlledEstateTerritories.erase(&territory);
	estateTerritories.erase(&territory);
	siegingTerritories.erase(&territory);
	siegedTerritories.erase(&territory);

	if(grid.containsGrid(territory.getGrid().getId()))
	{
		grid.removeGrid(territory.getGrid());
	}
}

void RealmTerritories::addControl(Territory& territory)
{
	assert(siegingTerritories.count(&territory) == 0);

	siegedTerritories.erase(&territory);
	controlledTerritories.insert(&territory);
	if(estateTerritories.count(&territory) == 1)
	{
		controlledEstateTerritories.insert(&territory);
	}

	const GridType gridType = territory.getType() == TerritoryType::land ? GridType::land : GridType::naval;
	if(grid.containsGrid(territory.getGrid().getId()))
	{
		grid.setGridType(territory.getGrid().getId(), gridType);
	}
	else
	{
		grid.addGrid(territory.getGrid(), gridType);
	}
}

void RealmTerritories::removeControl(const Territory& territory)
{
	assert(controlledTerritories.count(const_cast<Territory*>(&territory)) == 1);

	controlledTerritories.erase(const_cast<Territory*>(&territory));
	siegingTerritories.erase(const_cast<Territory*>(&territory));
	controlledEstateTerritories.erase(const_cast<Territory*>(&territory));

	grid.removeGrid(territory.getGrid());
}

void RealmTerritories::addEstateTerritory(Territory& territory)
{
	estateTerritories.insert(&territory);
	if(controlledTerritories.count(&territory) == 1)
	{
		controlledEstateTerritories.insert(&territory);
	}
}

void RealmTerritories::removeEstateTerritory(const Territory& territory)
{
	assert(estateTerritories.count(const_cast<Territory*>(&territory)) == 1);

	estateTerritories.erase(const_cast<Territory*>(&territory));
	controlledEstateTerritories.erase(const_cast<Territory*>(&territory));
	siegedTerritories.erase(const_cast<Territory*>(&territory));
}


void RealmTerritories::addSiegingTerritory(Territory& territory)
{
	assert(territory.getType() == TerritoryType::land);
	assert(controlledTerritories.count(const_cast<Territory*>(&territory)) == 1);
	assert(estateTerritories.count(const_cast<Territory*>(&territory)) == 0);

	siegingTerritories.insert(&territory);

	grid.setGridType(territory.getGrid().getId(), GridType::landSiege);
}

void RealmTerritories::removeSiegingTerritory(const Territory& territory)
{
	assert(territory.getType() == TerritoryType::land);
	assert(controlledTerritories.count(const_cast<Territory*>(&territory)) == 1);
	assert(siegingTerritories.count(const_cast<Territory*>(&territory)) == 1);

	siegingTerritories.erase(const_cast<Territory*>(&territory));
	grid.setGridType(territory.getGrid().getId(), GridType::land);
}

void RealmTerritories::addSiegedTerritory(Territory& territory)
{
	assert(territory.getType() == TerritoryType::land);
	assert(controlledTerritories.count(&territory) == 0);
	assert(controlledEstateTerritories.count(&territory) == 0);
	assert(estateTerritories.count(&territory) == 1);

	siegedTerritories.insert(&territory);
}

const std::unordered_set<Territory*>& RealmTerritories::getControlledTerritories() const
{
	return controlledTerritories;
}

const std::unordered_set<Territory*>& RealmTerritories::getControlledEstateTerritories() const
{
	return controlledEstateTerritories;
}

const std::unordered_set<Territory*>& RealmTerritories::getEstateTerritories() const
{
	return estateTerritories;
}

const std::unordered_set<Territory*>& RealmTerritories::getSiegingTerritories() const
{
	return siegingTerritories;
}

const std::unordered_set<Territory*>& RealmTerritories::getSiegedTerritories() const
{
	return siegedTerritories;
}

bool RealmTerritories::controlsTerritory(const Territory& territory) const
{
	return controlledTerritories.count(const_cast<Territory*>(&territory)) == 1;
}

bool RealmTerritories::testInvariants() const
{
	assert(controlledTerritories.size() >= controlledEstateTerritories.size());
	assert(estateTerritories.size() - controlledEstateTerritories.size() == siegedTerritories.size());
	assert(controlledTerritories.size() == controlledEstateTerritories.size() + siegingTerritories.size());

	for(const auto territory : siegingTerritories)
	{
		assert(controlledTerritories.count(territory) == 1);
	}

	return true;
}

bool territoriesDisjoint(const RealmTerritories& territories1, const RealmTerritories& territories2)
{
	for(Territory* territory1 : territories1.getControlledTerritories())
	{
		if(territories2.getControlledTerritories().count(territory1) == 1)
		{
			return false;
		}
	}

	return true;
}
