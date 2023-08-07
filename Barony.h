#pragma once
#include "LandedEstate.h"
#include "LandTerritory.h"
#include <memory>

class CoastalTerritory;
class LandTerritory;

class Barony : public LandedEstate
{
public:
	Barony(LandTerritory &territory, double landArmyYield);
	Barony(LandTerritory &territory, double landArmyYield, double navalFleetYield);
	Barony(Player *ruler, LandTerritory &territory, double landArmyYield);
	Barony(Player *ruler, LandTerritory &territory, double landArmyYield, double navalFleetYield);

	~Barony() = default;

	virtual std::shared_ptr<LandArmy> yieldLandArmy() override;
	virtual std::shared_ptr<NavalFleet> yieldNavalFleet() override;

private:
	double landArmyYield;  // Yield of land army per turn.
	double navalFleetYield = 0;  // Yield of naval fleets per turn.

	double cumulativeLandArmy = 0;  // Cumulation of land army yields.
	double cumulativeNavalFleet = 0;  // Cumulation of naval fleet yields.

	const bool canYieldNavy = false;
};

