#pragma once
#include "Subject.h"
#include "EditorGrid.h"
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

enum class TerritoryDrawMode
{
	terrain,
	culture,
	prosperity
};

class Territory : public Subject
{
public:
	Territory(int id, EditorGrid grid, TerritoryType type);  // Temporary for testing.
	Territory(int id, sf::Color color, TerritoryType type);  // Temporary for testing.
	Territory(int id, EditorGrid grid, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type);
	Territory(int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler, TerritoryType type);
	virtual ~Territory() = default;

	virtual void saveToFile(std::ofstream &file) const;

	virtual void draw(sf::RenderWindow &window) const; 

	void assignLandedEstate(LandedEstate *estate);

	virtual void calculateDistances(const std::vector<Territory*> &territories);

	virtual void setDrawMode(TerritoryDrawMode mode);
	TerritoryDrawMode getDrawMode() const;

	IOccupiable* getOccupancyHandler();
	const IOccupiable* getOccupancyHandler() const;

	TerritoryDistanceMap &getDistanceMap();
	const TerritoryDistanceMap &getDistanceMap() const;

	EditorGrid& getGrid();
	const EditorGrid& getGrid() const;

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
	EditorGrid grid;
	TerritoryDrawMode drawMode;
	TerritoryType type;
	LandedEstate *landedEstate = nullptr; 
	std::unique_ptr<IOccupiable> occupancyHandler;  // Handles military occupancy of territory.
	TerritoryDistanceMap distanceMap;  // Stores information about distances to any territory.
};

int loadTerritoryID(std::ifstream &file);

