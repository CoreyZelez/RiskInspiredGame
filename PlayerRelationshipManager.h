#pragma once
#include <vector>

class Player;

class PlayerRelationshipManager
{
public:

private:
	Player *liege;
	std::vector<Player*> vassals;
};

