#pragma once
#include "EditorGrid.h"
#include "LandTerritoryFeatures.h"
#include "Port.h"
#include "EditorPort.h"
#include "Territory.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <set>

class Player;
class EditorEstate;

class EditorTerritory 
{
public:
	EditorTerritory(TerritoryType type, int id);  
	EditorTerritory(TerritoryType type, int id, const EditorGrid &grid);
	EditorTerritory(TerritoryType type, int id, const EditorGrid& grid, 
		LandTerritoryFeatures landTerritoryFeatures);

	void saveToFile(std::ofstream& file) const;

	void draw(sf::RenderWindow& window) const;
	void drawPort(sf::RenderWindow& window) const;

	void assignLandedEstate(EditorEstate* estate);

	virtual void setDrawMode(TerritoryDrawMode mode);

	bool isAdjacent(const EditorTerritory& territory) const;

	void createPort(EditorTerritory& navalTerritory);

	TerritoryDrawMode getDrawMode() const;
	EditorGrid& getGrid();
	const EditorGrid& getGrid() const;

	TerritoryType getType() const;
	int getID() const;

	// Save label is identifier in txt file for territory type.
	std::string getSaveLabel() const;

	LandTerritoryFeatures& getLandFeatures();

private:
	int id;  // ID representing territory in text file.
	EditorGrid grid;
	TerritoryDrawMode drawMode;
	TerritoryType type;
	EditorEstate* landedEstate = nullptr;

	LandTerritoryFeatures landTerritoryFeatures;  // Features of territory if type is land.
	std::unique_ptr<EditorPort> port = nullptr;
};

void adjustLandTerrain(EditorTerritory& landTerritory, const Terrain& terrain);

void adjustLandCulture(EditorTerritory& landTerritory, const Culture& culture);

// Adjusts both prosperity and core prosperity.
void adjustLandProsperities(EditorTerritory& landTerritory, const int& prosperity);



