#pragma once
#include "Title.h"
#include "LandArmy.h"
#include <vector>

class Player;

class Estate
{
public:
	Estate(Title title, const Player *ruler);

	Title getTitle() const;

protected:
	void changeRuler(const Player *ruler);

private:
	const Title title;
	std::vector<Estate*> subFiefs;
	const Player *ruler;
};

