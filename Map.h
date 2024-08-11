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
	Map(const GameplaySettings& gameSettings, std::string name);

	TerritoryManager &getTerritoryManager();
	const TerritoryManager &getTerritoryManager() const;
	EstateManager &getEstateManager();
	const EstateManager &getEstateManager()const ;

private:
	void load(const GameplaySettings& gameSettings, std::string name);

	std::string name;
	TerritoryManager territoryManager;
	EstateManager estateManager;
};
