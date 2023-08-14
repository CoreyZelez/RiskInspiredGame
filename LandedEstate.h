#pragma once
#include "Estate.h"
#include "Observer.h"
#include "IMilitaryFactory.h"

class Territory;

class LandedEstate : public Estate, public Observer, public IMilitaryFactory
{
public:
	LandedEstate(Title title, const Player *ruler, Territory &territory);
	LandedEstate(Title title, Territory &territory);

	virtual void update(Message message) override;

	// Creates land army on some territory and returns handle to army.
	virtual std::shared_ptr<LandArmy> yieldLandArmy() override = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::shared_ptr<NavalFleet> yieldNavalFleet() override = 0;

	virtual bool containsPosition(const sf::Vector2f &position) const override;

protected:
	std::shared_ptr<LandArmy> putArmy(int strength);  // Puts new land army owned by ruler onto territory. Returns handle.
	std::shared_ptr<NavalFleet> putFleet(int strength);  // Puts new naval fleet owned by ruler onto territory. Returns handle.

private:
	Territory &territory;
};

