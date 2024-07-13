#include "Maridom.h"
#include "NavalTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"

Maridom::Maridom(NavalTerritory& territory)
	: Maridom(territory, sf::Color(220, 220, 220))
{
}

Maridom::Maridom(NavalTerritory& territory, const sf::Color &color)
	: LandedEstate(Title::maridom, territory, color)
{
}

void Maridom::update(Message message)
{
	// Observed territory occupant changed.
	if (message == Message::newController)
	{
		setOwnership(getTerritory().getController());
	}
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
