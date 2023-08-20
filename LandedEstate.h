#pragma once
#include "Estate.h"
#include "Observer.h"
#include "IMilitaryFactory.h"

class Territory;

class LandedEstate : public Estate, public Observer, public IMilitaryFactory
{
public:
	LandedEstate(Title title, Territory &territory);

	virtual void update(Message message) override;

	virtual void yield(PlayerMilitaryManager &militaryManager) override;

	// Creates land army on some territory and returns handle to army.
	virtual std::shared_ptr<LandArmy> yieldLandArmy() = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::shared_ptr<NavalFleet> yieldNavalFleet() = 0;

	virtual bool containsPosition(const sf::Vector2f &position) const override;

protected:
	// Yields military units at territory if possible.
	virtual void generateMilitary(PlayerMilitaryManager &militaryManager) override;  
	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override = 0;

	// Puts new land army owned by ruler onto territory. Returns handle.
	std::shared_ptr<LandArmy> putArmy(int strength);  
	// Puts new naval fleet owned by ruler onto territory. Returns handle.
	std::shared_ptr<NavalFleet> putFleet(int strength);  

private:
	Territory &territory;
};

