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

	// Creates land army on some territory and returns handle to army.
	virtual std::shared_ptr<LandArmy> yieldLandArmy();
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::shared_ptr<NavalFleet> yieldNavalFleet();

protected:
	void setRuler(const Player *ruler);

private:
	const Title title;
	std::vector<Estate*> subFiefs;
	const Player *ruler;
};

