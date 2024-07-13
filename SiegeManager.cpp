#include "SiegeManager.h"
#include "LandArmy.h"
#include "Player.h"
#include "Barony.h"

SiegeManager::SiegeManager(Barony& barony, double resistanceStrength)
	: barony(barony), resistanceStrength(resistanceStrength)
{
}

void SiegeManager::update(Message message)
{
	if(message == vacatedMilitary)
	{
		siegeLifted();
	}
}

void SiegeManager::update()
{
	if (resistanceStrength >= siegingArmy->getTotalStrength())
	{
		//reduce sieging army strength a lot.
		//reduce resistance strength a little
	}
	else
	{
		// reduce resistance strength a lot.
		// reduce sieging army strength a little
	}

	// REDUCIONS ACROSS ABOVE TWO CASES MUST BE CONTINUOUS IGNORING RANDOMNESS.

	// When sieging army dies the update observer function is called which handles siege removal.
	assert(siegingArmy == nullptr || !siegingArmy->isDead());

	// Handle sieging army taking over barony.
	if (resistanceStrength == 0 && !siegingArmy->isDead())
	{
		// Each players realms will be updated to handle the end of the siege, through the changing of ownership
		// of the barony.
		barony.setOwnership(&siegingArmy->getOwner());
		removeSiegingArmy();

	}
	else if(siegingArmy->isDead())
	{
		// Do nothing. Siege lifted through observer update function call.
	}
}

void SiegeManager::initSiege(LandArmy& army)
{
	assert(siegingArmy == nullptr);
	assert(&army.getOwner() != barony.getRuler());

	siegingArmy = &army;
	siegingArmy->setSiegeBarony(&barony);
	siegingArmy->addObserver(this);
	
	army.getOwner().getRealm().handleBaronySiegeBegin(barony, true);
	barony.getRuler()->getRealm().handleBaronySiegeBegin(barony, false);
}

bool SiegeManager::activeSiege() const
{
	return siegingArmy != nullptr;
}

void SiegeManager::siegeLifted()
{
	siegingArmy->getOwner().getRealm().handleBaronySiegeLifted(barony, true);
	barony.getRuler()->getRealm().handleBaronySiegeLifted(barony, false);
	removeSiegingArmy();
}

void SiegeManager::removeSiegingArmy()
{
	siegingArmy->removeObserver(this);
	siegingArmy->setSiegeBarony(nullptr);
	siegingArmy = nullptr;
}
