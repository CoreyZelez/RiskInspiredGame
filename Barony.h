#pragma once
#include "Observer.h"
#include "LandedEstate.h"
#include "LandTerritory.h"
#include "SiegeManager.h"
#include <memory>
#include <SFML/Graphics.hpp>

class CoastalTerritory;
class LandTerritory;


class Barony : public LandedEstate, public Observer
{
public:
	Barony(LandTerritory &landTerritory, sf::Color color);

	virtual ~Barony() = default;

	virtual void update(Message message) override;

	void updateSiege();

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
	SiegeManager siegeManager;

	double cumulativeLandArmy = 0;  // Cumulation of land army yields.
	double cumulativeNavalFleet = 0;  // Cumulation of naval fleet yields.
};

