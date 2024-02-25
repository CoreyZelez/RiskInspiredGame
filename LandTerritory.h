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
	LandTerritory(int id, Grid graphics, LandTerritoryFeatures features, NavalTerritory *navalTerritory);
	LandTerritory(int id, Grid graphics, NavalTerritory *navalTerritory);
	LandTerritory(int id, Grid graphics);
	explicit LandTerritory(int id);

	virtual ~LandTerritory() = default;

	virtual void saveToFile(std::ofstream &file) const override;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

	virtual void setDrawMode(TerritoryDrawMode mode) override;
	void setTerrain(Terrain terrain);

	std::unique_ptr<Port> &getPort();
	void drawPort(sf::RenderWindow &window) const;
	void createPort(NavalTerritory &navalTerritory);
	void deletePort();
	bool hasPort() const;

private:
	LandTerritoryFeatures features;
	std::unique_ptr<Port> port = nullptr;

	void initDefaultFeatures();
};

int loadPortNavalID(std::ifstream &file);

LandTerritoryFeatures loadLandTerritoryFeatures(std::ifstream &file);