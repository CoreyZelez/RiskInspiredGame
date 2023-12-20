#include "PlayerEstateManager.h"
#include "Estate.h"
#include "Utility.h"
#include "LandedEstate.h"
#include "RealmGrid.h"
#include <iostream>

PlayerEstateManager::PlayerEstateManager(RealmGrid & realmGrid, MilitaryManager & militaryManager)
	: realmGrid(realmGrid), militaryManager(militaryManager)
{
}

void PlayerEstateManager::handleMilitaryYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(Estate* estate : estates)
	{
		estate->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units and potentially always just military units).
	// Use a copy of fiefs due to possible resizing when yielding troops as in particular
	// naval units may occupy an unowned naval territory.
	std::vector<Estate*> preEstates = estates;
	for(Estate* fief : preEstates)
	{
		fief->yield(militaryManager);
	}
}

void PlayerEstateManager::addEstate(Estate *estate)
{
	assert(estate != nullptr);

	estates.emplace_back(estate);

	// Only update grid if estate landed.
	if(dynamic_cast<LandedEstate*>(estate) != nullptr)
	{
		realmGrid.addGrid(estate->getGrid());
	}

	// Add territory to realm territories if estate is landed estate.
	if(LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(estate))
	{
		territories.insert(&landedEstate->getTerritory());
	}
}

void PlayerEstateManager::removeEstate(Estate *estate)
{
	assert(estate != nullptr);

	for(auto iter = estates.begin(); iter != estates.end(); ++iter)
	{
		if(*iter == estate)
		{
			// Only update grid if estate landed.
			if(dynamic_cast<LandedEstate*>(estate) != nullptr)
			{
				realmGrid.removeGrid(estate->getGrid());
			}

			// Remove territory from realm territories if estate is landed estate.
			LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(*iter);
			if(landedEstate != nullptr)
			{
				assert(territories.count(&landedEstate->getTerritory()) == 1);
				territories.erase(&landedEstate->getTerritory());
			}

			estates.erase(iter);

			return;
		}
	}

	assert(false);  // Functions should only be called when the estate owner is this player.
}

const std::unordered_set<Territory*>& PlayerEstateManager::getTerritories()
{
	return territories;
}

std::map<Title, int> PlayerEstateManager::getTitleCounts() const
{
	std::map<Title, int> counts;
	for(const auto &estate : estates)
	{
		++counts[estate->getTitle()];
	}
	return counts;
}
