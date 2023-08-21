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

	void addFief(std::shared_ptr<Estate> estate);
	void removeFief(const Estate *fief);

	PlayerRelationshipManager &getRelationshipManager();

private:
	Title title;  // Official title of player.
	std::vector<std::shared_ptr<Estate>> fiefs;
	PlayerMilitaryManager militaryManager;
	PlayerRelationshipManager relationshipManager;
};

