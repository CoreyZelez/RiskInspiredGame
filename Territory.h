#pragma once
#include "TerritoryGraphics.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player;
class LandArmy;
class NavalFleet;

class Territory 
{
public:
	void draw(sf::RenderWindow &window) const;  // TEMPORARY. 

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) = 0;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) = 0;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) = 0;

	void addGridPosition(sf::Vector2f position);

	double getDefenceMultiplier() const;

private:
	TerritoryGraphics graphics;
	double defenceMultiplier = 1;  // In future perhaps have complex virtual function to calculate this!
};

