#pragma once
#include "Culture.h"
#include "Terrain.h"
#include <SFML/Graphics.hpp>

struct LandTerritoryFeatures
{
	int prosperity;  // Current prosperity of territory. Core prosperity tends to this slowly.
	int coreProsperity;  // Underlying prosperity of territory. Prosperity tends to this quickly.
	Culture culture;
	Terrain terrain;
};

