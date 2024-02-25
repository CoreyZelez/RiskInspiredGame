#pragma once
#include "Culture.h"
#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <fstream>

struct LandTerritoryFeatures
{
	void saveToFile(std::ofstream &file) const;

	int prosperity;  // Current prosperity of territory. Core prosperity tends to this slowly.
	int coreProsperity;  // Underlying prosperity of territory. Prosperity tends to this quickly.
	Terrain terrain;
	Culture culture;
};

