#pragma once
#include "Estate.h"
#include "Observer.h"

class Territory;

class LandedEstate : public Estate, public Observer
{
public:
	LandedEstate(Title title, const Player *ruler, Territory &territory);

	virtual void update(Message message) override;

	// virtual std::vector<std::shared_ptr<LandArmy>> yieldLandArmies();
	// virtual std::vector<std::shared_ptr<NavalFleet>> yieldNavalFleets();

protected:
	void putArmy(int strength);  // Puts new land army owned by ruler onto territory.
	void putFleet(int strength);  // Puts new naval fleet owned by ruler onto territory.

private:
	Territory &territory;
};

