#include "RealmEstateManager.h"
#include "Estate.h"
#include "Utility.h"

RealmEstateManager::RealmEstateManager(MilitaryManager &militaryManager)
	: militaryManager(militaryManager)
{
	grid.setColor(createRandomRealmColor());
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
	assert(fief->compareRuler(&this->player));
	if(updateGrid)
	{
		grid.addGrid(fief->getGrid());
	}
}

void RealmEstateManager::removeFief(const Estate *fief, bool updateGrid)
{
	for(auto iter = fiefs.begin(); iter != fiefs.end(); ++iter)
	{
		if(*iter == fief)
		{
			fiefs.erase(iter);
			if(updateGrid)
			{
				grid.removeGrid(fief->getGrid());
			}
			return;
		}
	}
	assert(false);  // Functions should only be called when the estate owner is this player.
}

bool RealmEstateManager::containsPosition(const sf::Vector2f &position) const
{
	return grid.containsPosition(position);
}

int RealmEstateManager::getRealmSize() const
{
	return fiefs.size();
}
