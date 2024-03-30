#pragma once
#include "Culture.h"
#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <fstream>

class GameplaySettings;

struct LandTerritoryFeatures
{
	LandTerritoryFeatures(const GameplaySettings *gameplaySettings);

	void saveToFile(std::ofstream &file) const;
	
	double calculateArmyYield() const;
	double calculateFleetYield() const;

	const GameplaySettings *gameplaySettings;
	double prosperity;  // Current prosperity of territory. Core prosperity tends to this slowly.
	double coreProsperity;  // Underlying prosperity of territory. Prosperity tends to this quickly.
	Terrain terrain = { -1, "temporary", sf::Color(40, 40, 40), 0, 0 };
	Culture culture = { -1, "temporary", sf::Color(40, 40, 40) };
};

sf::Color determineProsperityColor(double prosperity);


