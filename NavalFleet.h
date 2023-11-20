#pragma once
#include "MilitaryForce.h"

class NavalFleet : public MilitaryForce
{
public:
	NavalFleet(Player &owner, Territory *location, int strength);
	NavalFleet(Player &owner, Territory *location, std::array<unsigned int, 4> staminaStrength);
	~NavalFleet() = default;

	virtual void move(Territory &location, unsigned int strength) override;
	virtual void moveClosest(Territory &target, unsigned int strength, int maxDist) override;

	// void attack(NavalFleet &defendingArmy, /// double defenceMultiplier);

protected:
	virtual std::pair<int, int> calculateMinMaxStaminaCost(const Territory &territory) const override;

private:

};