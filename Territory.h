#pragma once
#include "Subject.h"
#include "Grid.h"
#include "IOccupiable.h"
#include "TerritoryDistanceMap.h"
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

	void draw(sf::RenderWindow &window) const;  // In future consdier returning vertex arrays probably!!!

	void assignLandedEstate(const LandedEstate *estate);

	virtual void calculateDistances(const std::vector<Territory*> &territories);

	IOccupiable* getOccupancyHandler();

	TerritoryDistanceMap &getDistanceMap();
	const TerritoryDistanceMap &getDistanceMap() const;

	Grid& getGrid();
	const Grid& getGrid() const;

	TerritoryType getType() const;

	int getID() const;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0; 	

	// Returns owner of associated estate.
	const Player *getEstateOwner() const;  

private:
	int id;  // ID representing territory in text file.
	Grid grid;
	TerritoryType type;
	const LandedEstate *landedEstate = nullptr; 
	std::unique_ptr<IOccupiable> occupancyHandler;  // Handles military occupancy of territory.
	TerritoryDistanceMap distanceMap;  // Stores information about distances to any territory.
};

int loadTerritoryID(std::ifstream &file);

