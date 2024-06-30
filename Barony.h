#pragma once
#include "LandedEstate.h"
#include "LandTerritory.h"
#include <memory>
#include <SFML/Graphics.hpp>

class CoastalTerritory;
class LandTerritory;


class Barony : public LandedEstate
{
public:
	Barony(LandTerritory &landTerritory, sf::Color color);

	virtual ~Barony() = default;

	virtual std::unique_ptr<LandArmy> yieldLandArmy() override;
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() override;

protected:
	// Puts new naval fleet owned by ruler onto naval territory associated with port. Returns handle.
	virtual std::unique_ptr<NavalFleet> putFleet(int strength) override;

	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override;

	virtual std::string getSaveLabel() const override;

private:
	LandTerritory &landTerritory;

	double cumulativeLandArmy = 0;  // Cumulation of land army yields.
	double cumulativeNavalFleet = 0;  // Cumulation of naval fleet yields.
};

