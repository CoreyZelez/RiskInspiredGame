#pragma once
#include "Title.h"
#include <vector>

class Fief;

class Player
{
public:

private:
	Title title;  // Official title of player.
	std::vector<Fief*> fiefs;

	Player *liege;
	double levies;
};

