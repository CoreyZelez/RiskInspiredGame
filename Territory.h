#pragma once
#include "Subject.h"
#include "TerritoryGrid.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player;
class LandArmy;
class NavalFleet;

const std::string landSaveLabel = "# land";
const std::string navalSaveLabel = "# naval";

class Territory : public Subject
{
public:
	Territory(int id, TerritoryGrid grid);
	Territory(int id, sf::Color color);

	void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;  // In future return vertex arrays probably!!!

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) = 0;

	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;

	double getDefenceMultiplier() const;
	TerritoryGrid& getGrid();
	virtual const Player *getOccupant();
	bool isEmpty() const;  	// True if territory occupies no positions on map.
	virtual std::string getSaveLabel() const = 0; 	// Save label is identifier in txt file for territory type.
	sf::Vector2f getCenter() const;

private:
	int id;
	TerritoryGrid grid;
	double defenceMultiplier = 1;  // In future perhaps have complex virtual function to calculate this!
};

int loadTerritoryID(std::ifstream &file);


