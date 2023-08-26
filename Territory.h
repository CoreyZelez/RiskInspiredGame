#pragma once
#include "Subject.h"
#include "Grid.h"
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
	Territory(int id, Grid grid);
	Territory(int id, sf::Color color);

	void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;  // In future return vertex arrays probably!!!

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(LandArmy *army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(NavalFleet *fleet) = 0;

	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;

	double getDefenceMultiplier() const;

	Grid& getGrid();
	const Grid& getGrid() const;

	int getID() const;
	virtual Player *getOccupant();
	bool isEmpty() const;  	// True if territory occupies no positions on map.
	virtual std::string getSaveLabel() const = 0; 	// Save label is identifier in txt file for territory type.

protected:
	sf::Vector2f getCenter() const;

private:
	int id;
	Grid grid;
	double defenceMultiplier = 1;  // In future perhaps have complex virtual function to calculate this!
};

int loadTerritoryID(std::ifstream &file);


