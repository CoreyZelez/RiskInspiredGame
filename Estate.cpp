#include "Estate.h"
#include "Player.h"
#include <assert.h>

Estate::Estate(Title title, const Player *ruler)
	: title(title), ruler(ruler)
{
}

Title Estate::getTitle() const
{
	return title;
}

std::shared_ptr<LandArmy> Estate::yieldLandArmy()
{
	return nullptr;
}

std::shared_ptr<NavalFleet> Estate::yieldNavalFleet()
{
	return nullptr;
}

void Estate::setRuler(const Player *ruler)
{
	this->ruler = ruler;
}




