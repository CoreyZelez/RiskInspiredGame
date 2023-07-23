#pragma once
#include "Title.h"
#include "LandArmy.h"
#include <vector>

class Player;

class Fief
{
public:
	Fief(Title title, Player *ruler);

	virtual LandArmy generateLandArmy() const {};

	Title getTitle() const;

private:
	const Title title;
	std::vector<Fief*> subFiefs;
	Player *ruler;
};

