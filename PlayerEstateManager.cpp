#include "PlayerEstateManager.h"
#include "Estate.h"
#include "Utility.h"
#include "LandedEstate.h"
#include "MilitaryManager.h"
#include "Player.h"
#include "Barony.h"
#include <unordered_set>
#include <iostream>

PlayerEstateManager::~PlayerEstateManager()
{
}

void PlayerEstateManager::clearAllMaridomOwnership()
{
	for(Estate *estate : estates)
	{
		assert(estate->getTitle() == Title::maridom);
		estate->clearOwnership();
	}
	estates.clear();
	// realmgrid.clear
}

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
}

void PlayerEstateManager::removeEstate(const Estate &estate)
{
	for(auto iter = estates.begin(); iter != estates.end(); ++iter)
	{
		if(*iter == &estate)
		{
			estates.erase(iter);
			return;
		}
	}

	assert(false);  // Functions should only be called when the estate owner is this player.
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

int PlayerEstateManager::calculateArmySoftCapContribution() const
{
	// Army soft cap contribution per barony subfief (indirect or direct) of specified estate title.
	std::map<Title, int> titleContributionsPerBaronySubfief;
	titleContributionsPerBaronySubfief[Title::barony] = 20;  // Consider barony as it's own barony subfief.
	titleContributionsPerBaronySubfief[Title::county] = 5;
	titleContributionsPerBaronySubfief[Title::duchy] = 4;
	titleContributionsPerBaronySubfief[Title::kingdom] = 3;
	titleContributionsPerBaronySubfief[Title::empire] = 2;

	int softCapContribution = 0;
	for(const Estate *estate : estates)
	{
		// Number of direct or indirect barony subfiefs.
		int numBaronySubfiefs; 
		if(estate->getTitle() == Title::barony)
		{
			numBaronySubfiefs = 1;
		}
		else
		{
			numBaronySubfiefs = estate->calculateNumberOfSubfiefs(Title::barony, true);
		}
		const int contribution = (double)numBaronySubfiefs * titleContributionsPerBaronySubfief[estate->getTitle()];
		softCapContribution += contribution;
	}

	return softCapContribution;
}

int PlayerEstateManager::calculateFleetSoftCapContribution() const
{
	// Army soft cap contribution per barony subfief (indirect or direct) of specified estate title.
	std::map<Title, int> titleContributionsPerBaronySubfief;
	titleContributionsPerBaronySubfief[Title::barony] = 5;  // Consider barony as it's own barony subfief.
	titleContributionsPerBaronySubfief[Title::county] = 3;
	titleContributionsPerBaronySubfief[Title::duchy] = 2;
	titleContributionsPerBaronySubfief[Title::kingdom] = 1;
	titleContributionsPerBaronySubfief[Title::empire] = 1;

	const int portContribution = 30;

	int softCapContribution = 0;
	for(const Estate *estate : estates)
	{
		// Number of direct or indirect barony subfiefs.
		int numBaronySubfiefs;
		if(estate->getTitle() == Title::barony)
		{
			// Barony counts as its own subfief.
			numBaronySubfiefs = 1;

			// Add contribution if barony associated territory contains port.
			const Barony *barony = dynamic_cast<const Barony*>(estate);
			assert(barony != nullptr);
			if(barony->hasPort())
			{
				softCapContribution += portContribution;
			}
		}
		else
		{
			numBaronySubfiefs = estate->calculateNumberOfSubfiefs(Title::barony, true);
		}
		const int contribution = (double)numBaronySubfiefs * titleContributionsPerBaronySubfief[estate->getTitle()];
		softCapContribution += contribution;
	}

	return softCapContribution;
}

void PlayerEstateManager::ammendUnlandedEstateOwnership()
{
	int index = 0;
	auto estatesCopy = estates;
	// Use estates copy due to potential resizing.
	for(Estate* estate : estatesCopy)
	{
		if(dynamic_cast<LandedEstate*>(estate) != nullptr)
		{
			continue;
		}
		assert(estate->getTitle() != Title::maridom && estate->getTitle() != Title::barony);
		estate->ammendOwnership();
	}
}

bool PlayerEstateManager::landedEstatesContainsPosition(const sf::Vector2f &position) const
{
	for(const Estate* estate : estates)
	{
		const LandedEstate *landedEstate = dynamic_cast<const LandedEstate*>(estate);
		if(landedEstate != nullptr && landedEstate->containsPosition(position))
		{
			return true;
		}
	}
	return false;
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
