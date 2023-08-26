#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;
class Territory;
class LandArmy;
class NavalFleet;

class MilitaryManager
{
public:
	void draw(sf::RenderWindow &window) const;

	void addLandArmy(std::unique_ptr<LandArmy> army);
	void addNavalFleet(std::unique_ptr<NavalFleet> fleet);

private:
	std::vector<std::unique_ptr<LandArmy>> armies;
	std::vector<std::unique_ptr<NavalFleet>> navies;
};

