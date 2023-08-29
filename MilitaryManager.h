#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "LandArmy.h"
#include "NavalFleet.h"

class Player;
class Territory;
class LandArmy;
class NavalFleet;

class MilitaryManager
{
public:
	void draw(sf::RenderWindow &window) const;

	// Returns pointer to military force at specified world position.
	MilitaryForce* getMilitary(sf::Vector2f position);

	void addLandArmy(std::unique_ptr<LandArmy> army);
	void addNavalFleet(std::unique_ptr<NavalFleet> fleet);

private:
	// BE CAREFUL DELETING FROM BELOW LISTS AS POINTERS POTENTIALLY HELD IN TERRITORIES!!!
	std::vector<MilitaryForce*> militaries;  // Holds handle to every army and navy in manager.
	std::vector<std::unique_ptr<LandArmy>> armies;  
	std::vector<std::unique_ptr<NavalFleet>> navies;
};

