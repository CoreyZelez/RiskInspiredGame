#include "RealmEstateManager.h"
#include "Estate.h"
#include "Utility.h"
#include "LandedEstate.h"

RealmEstateManager::RealmEstateManager(MilitaryManager &militaryManager)
	: militaryManager(militaryManager), grid(createRandomRealmColor())
{
}

void RealmEstateManager::draw(sf::RenderWindow &window) const
{
	grid.draw(window);
}

void RealmEstateManager::handleFiefYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(auto &fief : fiefs)
	{
		fief->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units).
	for(auto &fief : fiefs)
	{
		fief->yield(militaryManager);
	}
}

void RealmEstateManager::addFief(Estate *fief, bool updateGrid)
{
	fiefs.emplace_back(fief);
	if(updateGrid)
	{
		grid.addGrid(fief->getGrid());
	}

	// Add territory to realm territories if estate is landed estate.
	if(LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(fief))
	{
		territories.insert(&landedEstate->getTerritory());
	}
}

void RealmEstateManager::removeFief(Estate *fief, bool updateGrid)
{
	for(auto iter = fiefs.begin(); iter != fiefs.end(); ++iter)
	{
		if(*iter == fief)
		{
			if(updateGrid)
			{
				grid.removeGrid(fief->getGrid());
			}

			// Remove territory from realm territories if estate is landed estate.
			LandedEstate *landedEstate = dynamic_cast<LandedEstate*>(*iter);
			if(landedEstate != nullptr)
			{
				assert(territories.count(&landedEstate->getTerritory()) == 1);
				territories.erase(&landedEstate->getTerritory());
			}

			fiefs.erase(iter);

			return;
		}
	}

	assert(false);  // Functions should only be called when the estate owner is this player.
}

bool RealmEstateManager::containsPosition(const sf::Vector2f &position) const
{
	return grid.containsPosition(position);
}

std::set<Territory*>& RealmEstateManager::getTerritories()
{
	return territories;
}

int RealmEstateManager::getRealmSize() const
{
	return fiefs.size();
}

std::map<Title, int> RealmEstateManager::getTitleCounts() const
{
	std::map<Title, int> counts;
	for(const auto &fief : fiefs)
	{
		++counts[fief->getTitle()];
	}
	return counts;
}
