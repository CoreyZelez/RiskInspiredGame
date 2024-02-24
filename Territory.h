#pragma once
#include "Subject.h"
#include "Grid.h"
#include "IOccupiable.h"
#include "TerritoryDistanceMap.h"
#include "LandTerritoryFeatures.h"
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

enum class TerritoryType
{
	land,
	naval
};

class Territory : public Subject
{
public:
	Territory(int id, Grid grid, TerritoryType type);  // Temporary for testing.
	Territory(int id, sf::Color color, TerritoryType type);  // Temporary for testing.
	Territory(int id, Grid grid, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type);
	Territory(int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type);
	virtual ~Territory() = default;

	virtual void saveToFile(std::ofstream &file) const;

	virtual void draw(sf::RenderWindow &window) const; 

	void assignLandedEstate(LandedEstate *estate);

	virtual void calculateDistances(const std::vector<Territory*> &territories);

	IOccupiable* getOccupancyHandler();
	const IOccupiable* getOccupancyHandler() const;

	TerritoryDistanceMap &getDistanceMap();
	const TerritoryDistanceMap &getDistanceMap() const;

	Grid& getGrid();
	const Grid& getGrid() const;

	TerritoryType getType() const;

	int getID() const;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0; 	

	// Returns owner of associated estate.
	Player *getEstateOwner();  
	// Returns owner of associated estate.
	const Player *getEstateOwner() const;

private:
	int id;  // ID representing territory in text file.
	Grid grid;
	TerritoryType type;
	LandedEstate *landedEstate = nullptr; 
	std::unique_ptr<IOccupiable> occupancyHandler;  // Handles military occupancy of territory.
	TerritoryDistanceMap distanceMap;  // Stores information about distances to any territory.
	LandTerritoryFeatures features;
};

int loadTerritoryID(std::ifstream &file);

