#pragma once
#include "LandedEstate.h"

class NavalTerritory;

class Maridom : public LandedEstate
{
public:
	Maridom(NavalTerritory& territory);
	Maridom(NavalTerritory &territory, const sf::Color& color);

	// Handles changing of associated territory occupant.
	virtual void update(Message message) override;

	virtual std::unique_ptr<LandArmy> yieldLandArmy() override;
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() override;

protected:
	virtual void receiveBonusYield(const float &bonus) override;

	virtual std::string getSaveLabel() const override;

};

