#pragma once

struct GameplaySettings
{
	// Cost of occupying hostile and unowned land territories.
	int landHostileOccupancyCost = 3;

	// Multiplies losses from combat. 
	double lossMultiplier = 1;
};

