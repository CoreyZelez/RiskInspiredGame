#pragma once

struct GameplaySettings
{
	double armyYieldMultiplier = 0.5;
	double fleetYieldMultiplier = 0.5;

	// Multiplies losses from combat. 
	double combatLossMultiplier = 1;

	double siegeStrengthLossMultiplier = 1;
};

