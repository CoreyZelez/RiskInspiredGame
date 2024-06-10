#pragma once
#include "MilitaryForce.h"

struct Terrain;

class LandArmy : public MilitaryForce
{
public:
	LandArmy(Player &owner, Territory *location, int strength);
	LandArmy(Player &owner, Territory *location, StaminaArray staminaStrength);

	virtual void removeFromTerritory() override;

	virtual void move(Territory &territory, unsigned int strength) override;
	virtual void moveClosest(Territory &target, unsigned int strength, int maxDist) override;

	void attack(LandArmy &defendingArmy, const Terrain &terrain);

	static int getLandMoveStaminaCost();
	static int getAttackStaminaCost();

protected:
	virtual std::pair<int, int> calculateMinMaxStaminaCost(const Territory &territory) const override;

private:

};

