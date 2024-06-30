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

enum class TerritoryDrawMode
{
	terrain,
	culture,
	prosperity
};

enum class TerritoryType
{
	land,
	naval
};

class Territory : public Subject
{
public:
	Territory(TerritoryType type, int id, EditorGrid grid);
	Territory(TerritoryType type, int id, sf::Color color);
	Territory(TerritoryType type, int id, EditorGrid grid, std::unique_ptr<IOccupiable> occupancyHandler);
	Territory(TerritoryType type, int id, sf::Color color, std::unique_ptr<IOccupiable> occupancyHandler);
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

	int getID() const;

	TerritoryType getType() const;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0; 	

	// Returns owner of associated estate.
	Player *getEstateOwner();  
	// Returns owner of associated estate.
	const Player *getEstateOwner() const;

private:
	int id;  // ID representing territory in text file.
	TerritoryType type;
	EditorGrid grid;
	TerritoryDrawMode drawMode;
	LandedEstate *landedEstate = nullptr; 
	std::unique_ptr<IOccupiable> occupancyHandler;  // Handles military occupancy of territory.
	TerritoryDistanceMap distanceMap;  // Stores information about distances to any territory.
};

int loadTerritoryID(std::ifstream &file);


