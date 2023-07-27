#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player;
class LandArmy;
class NavalFleet;

class Territory 
{
public:
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) = 0;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) = 0;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) = 0;

	double getDefenceMultiplier() const;

private:
	std::vector<sf::Vector2f> positions;  // Positions on map territory occupys.

	double defenceMultiplier = 1;  // In future perhaps have complex virtual function to calculate this!
};

