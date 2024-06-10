#pragma once
#include "Territory.h"
#include "Observer.h"
#include "Subject.h"
#include "Port.h"
#include "LandTerritoryFeatures.h"
#include <memory>

class NavalFleet;
class LandArmy;
class NavalTerritory;

class LandTerritory : public Territory 
{
public:
	LandTerritory(int id, EditorGrid graphics, LandTerritoryFeatures features, NavalTerritory *navalTerritory = nullptr);
	LandTerritory(int id, EditorGrid graphics, NavalTerritory *navalTerritory);
	LandTerritory(int id, EditorGrid graphics);
	explicit LandTerritory(int id);

	virtual ~LandTerritory() = default;

	virtual void saveToFile(std::ofstream &file) const override;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

	virtual void setDrawMode(TerritoryDrawMode mode) override;

	void setTerrain(Terrain terrain);
	void setCulture(Culture culture);
	// Sets prosperity and core prosperity value to the same.
	void setProsperities(int prosperity);
	const LandTerritoryFeatures &getFeatures() const;

	std::unique_ptr<Port> &getPort();
	void drawPort(sf::RenderWindow &window) const;
	void createPort(NavalTerritory &navalTerritory);
	void deletePort();
	bool hasPort() const;

	const Terrain& getTerrain() const;

private:
	LandTerritoryFeatures features;
	std::unique_ptr<Port> port = nullptr;

	void initDefaultFeatures();
};

int loadPortNavalID(std::ifstream &file);

LandTerritoryFeatures loadLandTerritoryFeatures(std::ifstream &file, const GameplaySettings *gameplaySettings);


void adjustLandTerrain(LandTerritory &landTerritory, const Terrain& terrain);

void adjustLandCulture(LandTerritory &landTerritory, const Culture& culture);

// Adjusts both prosperity and core prosperity.
void adjustLandProsperities(LandTerritory &landTerritory, const int &prosperity);

