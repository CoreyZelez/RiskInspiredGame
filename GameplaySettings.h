#pragma once

struct GameplaySettings
{
	double armyYieldMultiplier = 0.5;
	double fleetYieldMultiplier = 0.5;

	// Cost of occupying hostile and unowned land territories.
	int landHostileOccupancyCost = 3;

	// Multiplies losses from combat. 
	double lossMultiplier = 1;
};

