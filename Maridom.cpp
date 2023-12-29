#include "Maridom.h"
#include "NavalTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
Maridom::Maridom(NavalTerritory &territory)
	: LandedEstate(Title::maridom, territory)
{
}

std::unique_ptr<LandArmy> Maridom::yieldLandArmy()
{
	return nullptr;
}

std::unique_ptr<NavalFleet> Maridom::yieldNavalFleet()
{
	return nullptr;
}

void Maridom::receiveBonusYield(const float & bonus)
{
	// Intentionally empty.
}

std::string Maridom::getSaveLabel() const
{
	return maridomSaveLabel;
}
