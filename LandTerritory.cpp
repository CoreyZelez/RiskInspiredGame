#include "LandTerritory.h"
#include "LandArmy.h"

bool LandTerritory::attemptOccupy(std::shared_ptr<LandArmy> &army)
{
	// Case armies have same owner.
	if(&(army.get()->getOwner()) == &(this->army.get()->getOwner()))
	{
		// Absorb strength of army into this->army.
		const int armyStrength = army.get()->getStrength();
		this->army.get()->adjustStrength(army.get()->getStrength());  
		army.get()->adjustStrength(-armyStrength);
		return true;
	}
	// Case armies have different owner.
	else
	{
		// Temporarily implemented as always attack. In future potentially not as there may be friendly sharing of troop territory.
		army.get()->attack(*this->army.get(), getDefenceMultiplier());
		// Attacking army occupys land if defending army killed.
		if(this->army.get()->isDead() && !army.get()->isDead())
		{
			this->army = army;

			return true;
		}
	}
	return false;  
}

bool LandTerritory::attemptOccupy(std::shared_ptr<NavalFleet> &fleet)
{
	return false;  
}

