#include "LandTerritory.h"
#include "LandArmy.h"
#include <assert.h>
#include <iostream>

bool LandTerritory::occupy(std::shared_ptr<LandArmy> &army)
{
	assert(army.get() != nullptr);

	/// return false if territory of army not adjacent to this land territory!

	if(this->army.get() == nullptr)
	{
		this->army = army;
		army.get()->setLocation(this);
		return true;
	}
	// Case armies have same owner.
	else if(&(army.get()->getOwner()) == &(this->army.get()->getOwner()))
	{
		// Absorb strength of army into this->army.
		const int armyStrength = army.get()->getStrength();
		this->army.get()->adjustStrength(army.get()->getStrength());  
		army.get()->adjustStrength(-armyStrength);  // Sets strength 0.
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
			army.get()->setLocation(this);
			return true;
		}
	}
	return false;  // WARNING CURRENTLY USELESS. IS THIS EVER NEEDED!!!
}

bool LandTerritory::occupy(std::shared_ptr<NavalFleet> &fleet)
{
	return false;  
}

/// CAN THIS FUNCTION BE MOVED TO BASE CLASS!!!!
void LandTerritory::putArmy(std::shared_ptr<LandArmy>& army)
{
	// this->army must have same owner as army or be nullptr.
	assert(this->army.get() == nullptr || &this->army.get()->getOwner() == &army.get()->getOwner());
	if(this->army.get() == nullptr)
	{
		this->army = army;
	}
}

void LandTerritory::putFleet(std::shared_ptr<NavalFleet>& fleet)
{
}

const std::shared_ptr<LandArmy>& LandTerritory::getArmy() const
{
	return army;
}


