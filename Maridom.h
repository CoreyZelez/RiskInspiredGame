#pragma once
#include "LandedEstate.h"
#include "LandedEstate.cpp"

class NavalTerritory;

class Maridom : public LandedEstate<NavalTerritory>
{
public:
	Maridom(NavalTerritory& territory);
	Maridom(NavalTerritory &territory, const sf::Color& color);

	virtual std::unique_ptr<LandArmy> yieldLandArmy() override;
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() override;

protected:
	virtual void receiveBonusYield(const float &bonus) override;

	virtual std::string getSaveLabel() const override;
};

