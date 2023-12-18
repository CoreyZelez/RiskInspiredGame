#include "RealmEstateManager.h"
#include "Estate.h"
#include "Utility.h"
#include "LandedEstate.h"
#include <iostream>

RealmEstateManager::RealmEstateManager(MilitaryManager &militaryManager)
	: militaryManager(militaryManager), defaultGridColor(createRandomRealmColor()), grid(defaultGridColor)
{
}

void RealmEstateManager::draw(sf::RenderWindow &window) const
{
	grid.draw(window);
}

void RealmEstateManager::updateGrid()
{
	grid.update();
}

void RealmEstateManager::setGridColor(const sf::Color &color)
{
	grid.setColor(color);
}

void RealmEstateManager::setGridColorDefault()
{
	grid.setColor(defaultGridColor);
}

void RealmEstateManager::handleFiefYields()
{
	// Provide bonus yields to fiefs contained in subfiefs under this player's control.
	for(Estate* fief : fiefs)
	{
		fief->provideSubfiefBonusYields();
	}

	// Yield resources (currently just military units and potentially always just military units).
	// Use a copy of fiefs due to possible resizing when yielding troops as in particular
	// naval units may occupy an unowned naval territory.
	std::vector<Estate*> preFiefs = fiefs;
	for(Estate* fief : preFiefs)
	{
		fief->yield(militaryManager);
	}
}

void RealmEstateManager::addFief(Estate *fief, bool updateGrid)
{
	assert(fief != nullptr);

	fiefs.emplace_back(fief);
	if(updateGrid)
	{
		grid.addGrid(fief->getGrid(), updateGrid);
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
				grid.removeGrid(fief->getGrid(), updateGrid);
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
