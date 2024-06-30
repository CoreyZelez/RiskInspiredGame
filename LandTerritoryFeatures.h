#pragma once
#include "Culture.h"
#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <fstream>

class GameplaySettings;

struct LandTerritoryFeatures
{
	LandTerritoryFeatures() = default;
	LandTerritoryFeatures(const GameplaySettings *gameplaySettings);

	void saveToFile(std::ofstream &file) const;
	
	/////////////////
	///////////////////
	///////// IN FUTURE THIS SHOULD BE HANDLED ENTIRELY BY BARONY.
	///////// THE BARONY WILL OBSERVE THESE NUMBERS PLUS ITS OWN FEATURESTO DETERMINE YIELDS.
	double calculateArmyYield() const;
	double calculateFleetYield() const;
	///////////////////////////////////////////

	const GameplaySettings *gameplaySettings = nullptr;
	double prosperity = 0;  // Current prosperity of territory. Core prosperity tends to this slowly.
	double coreProsperity = 0;  // Underlying prosperity of territory. Prosperity tends to this quickly.
	Terrain terrain = { -1, "temporary", sf::Color(40, 40, 40), 0, 0 };
	Culture culture = { -1, "temporary", sf::Color(40, 40, 40) };
};

sf::Color determineProsperityColor(double prosperity);

int getHighProsperityThreshold();


