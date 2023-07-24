#pragma once
#include "Title.h"
#include "MilitaryManager.h"
#include <vector>
#include <memory>

class Fief;
class LandArmy;
class NavalFleet;

class Player
{
public:
	MilitaryManager &getMilitaryManager();

private:
	Title title;  // Official title of player.
	std::vector<Fief*> fiefs;
	Player *liege;
	MilitaryManager militaryManager;

};

