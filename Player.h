#pragma once
#include "Title.h"
#include "MilitaryManager.h"
#include <vector>
#include <memory>

class Estate;
class LandArmy;
class NavalFleet;

class Player
{
public:
	MilitaryManager &getMilitaryManager();

private:
	Title title;  // Official title of player.
	std::vector<Estate*> fiefs;
	Player *liege;
	MilitaryManager militaryManager;

};

