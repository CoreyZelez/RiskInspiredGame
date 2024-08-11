#pragma once
#include "Subject.h"
#include "Grid.h"
#include "ITerritoryOccupancy.h"
#include "TerritoryDistanceMap.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <set>

class Player;
class LandArmy;
class NavalFleet;

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
	Territory(TerritoryType type, int id, Grid grid);
	Territory(TerritoryType type, int id, Grid grid, std::unique_ptr<ITerritoryOccupancy> occupancyHandler);
	virtual ~Territory() = default;

	virtual void draw(sf::RenderWindow &window) const; 

	virtual void calculateDistances(const std::vector<Territory*> &territories);

	virtual void setDrawMode(TerritoryDrawMode mode);
	TerritoryDrawMode getDrawMode() const;

	ITerritoryOccupancy* getOccupancyHandler();
	const ITerritoryOccupancy* getOccupancyHandler() const;

	TerritoryDistanceMap &getDistanceMap();
	const TerritoryDistanceMap &getDistanceMap() const;

	// Returns the upper liege of the controller.
	Player* getUpperController();
	// Returns the upper liege of the controller.
	const Player* getUpperController() const;
	Player* getController();
	const Player* getController() const;

	Grid& getGrid();
	const Grid& getGrid() const;

	int getId() const;

	TerritoryType getType() const;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0; 	

private:
	int id;  // ID representing territory in text file.
	TerritoryType type;
	Grid grid;
	TerritoryDrawMode drawMode;
	std::unique_ptr<ITerritoryOccupancy> occupancyHandler;  // Handles military occupancy of territory.
	TerritoryDistanceMap distanceMap;  // Stores information about distances to any territory.
};
 
int loadTerritoryID(std::ifstream &file);


