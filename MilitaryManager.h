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
	void addArmy(std::shared_ptr<LandArmy> army);
	void addNavy(std::shared_ptr<NavalFleet> navy);

private:
	std::vector<std::shared_ptr<LandArmy>> armies;
	std::vector<std::shared_ptr<NavalFleet>> navies;
};

