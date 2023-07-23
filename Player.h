#pragma once
#include "Title.h"
#include <vector>
#include <memory>

class Fief;
class LandArmy;
class NavalFleet;

class Player
{
public:
	void addArmy(std::unique_ptr<LandArmy> army);
	void addNavy(std::unique_ptr<NavalFleet> navy);

private:
	Title title;  // Official title of player.
	std::vector<Fief*> fiefs;
	Player *liege;
	std::vector<std::unique_ptr<LandArmy>> armies;
	std::vector<std::unique_ptr<NavalFleet>> navies;

};

