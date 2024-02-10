#pragma once
#include "MilitaryForce.h"

class LandArmy : public MilitaryForce
{
public:
	LandArmy(Player &owner, Territory *location, int strength);
	LandArmy(Player &owner, Territory *location, std::array<unsigned int, 4> staminaStrength);

	virtual void removeFromTerritory() override;

	virtual void move(Territory &territory, unsigned int strength) override;
	virtual void moveClosest(Territory &target, unsigned int strength, int maxDist) override;

	void attack(LandArmy &defendingArmy, double defenceMultiplier);

protected:
	virtual std::pair<int, int> calculateMinMaxStaminaCost(const Territory &territory) const override;

private:

};

