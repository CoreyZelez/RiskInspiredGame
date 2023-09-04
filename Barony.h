#pragma once
#include "LandedEstate.h"
#include "LandTerritory.h"
#include <memory>

class CoastalTerritory;
class LandTerritory;

class Barony : public LandedEstate
{
public:
	Barony(LandTerritory &territory, double landArmyYield, double navalFleetYield);

	virtual ~Barony() = default;

	virtual void saveToFile(std::ofstream &file) const override;

	void setIsCoastal(bool isCoastal);

	virtual std::unique_ptr<LandArmy> yieldLandArmy() override;
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() override;

protected:
	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override;

	virtual std::string getSaveLabel() const override;

private:
	double landArmyYield;  // Yield of land army per turn.
	double navalFleetYield = 0;  // Yield of naval fleets per turn.

	double cumulativeLandArmy = 0;  // Cumulation of land army yields.
	double cumulativeNavalFleet = 0;  // Cumulation of naval fleet yields.

	bool yieldsNavy = false;
};

