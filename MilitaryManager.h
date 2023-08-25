#pragma once
#include <vector>
#include <memory>

class Player;
class Territory;
class LandArmy;
class NavalFleet;

class MilitaryManager
{
public:
	void addLandArmy(std::shared_ptr<LandArmy> army);
	void addNavalFleet(std::shared_ptr<NavalFleet> fleet);

private:
	std::vector<std::shared_ptr<LandArmy>> armies;
	std::vector<std::shared_ptr<NavalFleet>> navies;
};

