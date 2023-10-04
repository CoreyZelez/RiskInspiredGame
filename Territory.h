#pragma once
#include "Subject.h"
#include "Grid.h"
#include "IOccupiable.h"
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
	Territory(int id, Grid grid);  // Temporary for testing.
	Territory(int id, sf::Color color);  // Temporary for testing.
	Territory(int id, Grid grid, std::unique_ptr<IOccupiable> occupancyHandler);
	Territory(int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler);

	virtual ~Territory() = default;

	void assignLandedEstate(const LandedEstate *estate);

	void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;  // In future return vertex arrays probably!!!

	// Calculates distance to each territory in territories and updates distances map.
	virtual void calculateDistances(const std::vector<Territory*> &territories);
	virtual int getDistance(const Territory &territory, bool sameType) const;

	// Adds territories from parameters that are adjacent (Have bordering grid squares).
	virtual void addAdjacencies(std::vector<Territory*> &territories);
	bool isAdjacent(const Territory *territory) const;
	virtual const std::set<Territory*> &getAdjacencies(bool sameType) const;
	virtual std::set<Territory*> &getAdjacencies(bool sameType);

	IOccupiable* getOccupancyHandler();
	Grid& getGrid();
	const Grid& getGrid() const;
	int getID() const;
	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0; 	
	// Returns owner of associated estate.
	const Player *getEstateOwner() const;  

protected:
	// Calculates distances of this territory to all other specified (optionally same type) territories. 
	std::map<const Territory*, int> calculateDistancesBFS(const std::vector<Territory*>& territories, bool sameType);

private:
	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;

	int id;  // ID representing territory in text file.
	Grid grid;
	std::set<Territory*> adjacencies;  // Adjacent territories.
	mutable std::map<const Territory*, int> distances;  // Distances between any territory type.
	const LandedEstate *landedEstate = nullptr; 
	std::unique_ptr<IOccupiable> occupancyHandler;  // Handles military occupancy of territory.
};

int loadTerritoryID(std::ifstream &file);

