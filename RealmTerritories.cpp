#include "RealmTerritories.h"
#include "Territory.h"
#include <assert.h>

// All territories in realm territories are either controlled or sieged. Thus to remove all the territories,
// we need only check these two types.
void RealmTerritories::removeTerritories(const RealmTerritories& territories)
{
	for(Territory* territory : territories.controlledTerritories)
	{
		removeTerritory(*territory);
	}

	for(Territory* territory : territories.siegedTerritories)
	{
		removeTerritory(*territory);
	}
}

void RealmTerritories::removeTerritory(const Territory& territory)
{
	estateTerritories.erase(const_cast<Territory*>(&territory));
	controlledEstateTerritories.erase(const_cast<Territory*>(&territory));
	controlledTerritories.erase(const_cast<Territory*>(&territory));
	siegingTerritories.erase(const_cast<Territory*>(&territory));
	siegedTerritories.erase(const_cast<Territory*>(&territory));
}

void RealmTerritories::addControlledEstateTerritory(Territory& territory)
{
	removeTerritory(territory);

	controlledTerritories.insert(&territory);
	controlledEstateTerritories.insert(&territory);
	estateTerritories.insert(&territory);
}

void RealmTerritories::addSiegingTerritory(Territory& territory)
{
	assert(estateTerritories.count(&territory) == 0);
	assert(controlledEstateTerritories.count(&territory) == 0);
	assert(siegedTerritories.count(&territory) == 0);
	assert(territory.getType() == TerritoryType::land);

	controlledTerritories.insert(&territory);
	siegingTerritories.insert(&territory);
}

void RealmTerritories::addSiegedTerritory(Territory& territory)
{
	assert(controlledTerritories.count(&territory) == 1);
	assert(siegingTerritories.count(&territory) == 0);
	assert(territory.getType() == TerritoryType::land);

	controlledTerritories.erase(&territory);
	controlledEstateTerritories.erase(&territory);
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
	for(auto& controlledTerritory : controlledTerritories)
	{
		if(controlledTerritory->getId() == territory.getId())
		{
			return true;
		}
	}

	return false;
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
