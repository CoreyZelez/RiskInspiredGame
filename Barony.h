#pragma once
#include "Observer.h"
#include "LandedEstate.h"
#include "LandedEstate.cpp"
#include "LandTerritory.h"
#include "SiegeManager.h"
#include <memory>
#include <SFML/Graphics.hpp>

class CoastalTerritory;
class LandTerritory;


class Barony : public LandedEstate<LandTerritory>
{
public:
	Barony(const GameplaySettings& gameplaySettings, LandTerritory &landTerritory, sf::Color color);

	virtual ~Barony() = default;

	void update();

	void updateSiege();

	virtual void setOwnership(Player* ruler, bool recurseOnParents = true) override;

	virtual std::unique_ptr<LandArmy> yieldLandArmy() override;
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() override;

protected:
	// Puts new naval fleet owned by ruler onto naval territory associated with port. Returns handle.
	virtual std::unique_ptr<NavalFleet> putFleet(int strength) override;

	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override;

	virtual std::string getSaveLabel() const override;

private:
	SiegeManager siegeManager;

	double cumulativeLandArmy = 0;  // Cumulation of land army yields.
	double cumulativeNavalFleet = 0;  // Cumulation of naval fleet yields.
};

