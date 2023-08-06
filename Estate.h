#pragma once
#include "Title.h"
#include <vector>
#include <memory>

class Player;
class LandArmy;
class NavalFleet;


class Estate
{
public:
	Estate(Title title, const Player *ruler);

	Title getTitle() const;

protected:
	const Player* getRuler() const;
	void setRuler(const Player *ruler);

private:
	const Title title;
	std::vector<Estate*> subFiefs;
	const Player *ruler;
};

