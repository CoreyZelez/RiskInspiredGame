#pragma once
#include "Title.h"
#include "PlayerMilitaryManager.h"
#include "PlayerRelationshipManager.h"
#include <vector>
#include <memory>

class Estate;
class LandArmy;
class NavalFleet;

class Player
{
public:
	Player();

	void handleFiefYields();

	void addFief(Estate *fief);
	void removeFief(const Estate *fief);

	PlayerRelationshipManager &getRelationshipManager();

private:
	Title title;  // Official title of player.
	std::vector<Estate*> fiefs;
	PlayerMilitaryManager militaryManager;
	PlayerRelationshipManager relationshipManager;
};

