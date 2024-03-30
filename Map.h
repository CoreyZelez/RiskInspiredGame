#pragma once
#include "TerritoryManager.h"
#include "EstateManager.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <SFML/Graphics.hpp>
#include <string>

class GameplaySettings;

class Map
{
public:
	Map(std::string name, const GameplaySettings *gameSettings = nullptr);

	void save();
	void saveAs(std::string name);  // Save map under different name.

	TerritoryManager &getTerritoryManager();
	const TerritoryManager &getTerritoryManager() const;
	EstateManager &getEstateManager();
	const EstateManager &getEstateManager()const ;

private:
	void load(std::string name, const GameplaySettings *gameSettings);

	std::string name;
	TerritoryManager territoryManager;
	EstateManager estateManager;
};
