#include "Player.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include <assert.h>

MilitaryManager& Player::getMilitaryManager()
{
	return militaryManager;
}
