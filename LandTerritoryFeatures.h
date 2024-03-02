#pragma once
#include "Culture.h"
#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <fstream>

struct LandTerritoryFeatures
{
	void saveToFile(std::ofstream &file) const;

	double prosperity;  // Current prosperity of territory. Core prosperity tends to this slowly.
	double coreProsperity;  // Underlying prosperity of territory. Prosperity tends to this quickly.
	Terrain terrain = { -1, "temporary", sf::Color(40, 40, 40), 0, 0 };
	Culture culture = { -1, "temporary", sf::Color(40, 40, 40) };
};

sf::Color determineProsperityColor(double prosperity);


