#pragma once
#include "LandedEstate.h"

class NavalTerritory;

/* Landed estate associated with Naval territories.*/
class Maridom : public LandedEstate
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

