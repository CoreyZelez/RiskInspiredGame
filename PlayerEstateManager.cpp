#include "PlayerEstateManager.h"
#include "Estate.h"
#include "Utility.h"
#include "LandedEstate.h"
#include "RealmGrid.h"
#include "MilitaryManager.h"
#include <unordered_set>
#include <iostream>

void PlayerEstateManager::handleMilitaryYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(Estate* estate : estates)
	{
		estate->provideSubfiefBonusYields();
	}

	// Yield resources.
	// Use a copy of fiefs due to possible resizing when yielding troops as in particular
	// naval units may occupy an unowned naval territory.
	std::unordered_set<Estate*> preEstates = estates;
	for(Estate* fief : preEstates)
	{
		fief->yield();
	}
}

void PlayerEstateManager::addEstate(Estate &estate)
{
	estates.insert(&estate);

	LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(&estate);
	// Add territory to realm territories if estate is landed estate.
	if(landedEstate != nullptr)
	{
		territories.insert(&landedEstate->getTerritory());
	}
}

void PlayerEstateManager::removeEstate(Estate &estate)
{
	for(auto iter = estates.begin(); iter != estates.end(); ++iter)
	{
		if(*iter == &estate)
		{
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

std::unordered_set<const Estate*> PlayerEstateManager::getEstates() const
{
	std::unordered_set<const Estate*> estates;
	for(auto iter = this->estates.begin(); iter != this->estates.end(); ++iter)
	{
		estates.insert(*iter);
	}
	return estates;
}

bool PlayerEstateManager::containsEstate(const Estate &estate) const
{
	return (estates.count(const_cast<Estate*>(&estate)) == 1);
}

std::map<Title, int> PlayerEstateManager::getTitleCounts() const
{
	std::map<Title, int> counts;

	for(const Estate* estate : estates)
	{
		++counts[estate->getTitle()];
	}

	return counts;
}

Title PlayerEstateManager::getHighestTitle() const
{
	std::map<Title, int> titleCounts = getTitleCounts();
	const std::vector<Title> orderedTitles = getOrderedTitles();
	// Iterate through titles in reverse order (highest to lowest) until highest held title is determined.
	for(int i = orderedTitles.size() - 1; i >= 0; --i)
	{
		if(titleCounts[orderedTitles[i]] > 0)
		{
			return orderedTitles[i];
		}
	}
	return Title::noTitle;
}
