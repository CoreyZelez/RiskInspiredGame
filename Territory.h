#pragma once
#include "Subject.h"
#include "Grid.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <set>

class Player;
class LandArmy;
class NavalFleet;
class LandedEstate;

const std::string landSaveLabel = "# land";
const std::string navalSaveLabel = "# naval";

class Territory : public Subject
{
public:
	Territory(int id, Grid grid);
	Territory(int id, sf::Color color);

	virtual ~Territory() = default;

	void assignLandedEstate(const LandedEstate *estate);

	void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;  // In future return vertex arrays probably!!!

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(LandArmy *army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(NavalFleet *fleet) = 0;

	// Calculates distance to each territory in territories and updates distances map.
	void calculateDistances(const std::vector<Territory*> &territories);
	int getDistance(const Territory &territory) const;

	void addAdjacencies(std::vector<Territory*> &territories);
	bool isAdjacent(const Territory *territory) const;
	const std::set<Territory*> &getAdjacencies() const;
	std::set<Territory*> &getAdjacencies();

	virtual double getDefenceMultiplier() const;

	Grid& getGrid();
	const Grid& getGrid() const;
	int getID() const;
	virtual Player *getOccupant();
	bool isEmpty() const;  	// True if territory occupies no positions on map.
	virtual std::string getSaveLabel() const = 0; 	// Save label is identifier in txt file for territory type.
	const Player *getEstateOwner() const;

protected:
	sf::Vector2f getCenter() const;

private:
	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;

	int id;
	Grid grid;
	double defenceMultiplier = 1.2;  // In future perhaps have complex virtual function to calculate this!
	std::set<Territory*> adjacencies;
	mutable std::map<const Territory*, int> distances;
	const LandedEstate *landedEstate = nullptr;
};

int loadTerritoryID(std::ifstream &file);


