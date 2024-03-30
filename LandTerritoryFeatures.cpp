#include "LandTerritoryFeatures.h"
#include "GameplaySettings.h"
#include <assert.h>
#include <iostream>

LandTerritoryFeatures::LandTerritoryFeatures(const GameplaySettings *gameplaySettings)
	: gameplaySettings(gameplaySettings)
{
}

void LandTerritoryFeatures::saveToFile(std::ofstream &file) const
{
	file << "# prosperity" << std::endl;
	file <<  prosperity << std::endl;

	file << "# core prosperity" << std::endl;
	file << coreProsperity << std::endl;

	file << "# terrain id" << std::endl;
	file << terrain.id << std::endl;

	file << "# culture id" << std::endl;
	file << culture.id << std::endl;
}

double LandTerritoryFeatures::calculateArmyYield() const
{
	// Gameplay settings must be known.
	assert(gameplaySettings != nullptr);

	double yield = 0;

	if(prosperity < 100)
	{
		yield = 0.5 + (prosperity / 100);  // Maximum is 1.5.
	}
	else if(prosperity < 200)
	{
		yield = 1.5 + ((prosperity - 100) / 50);  // Maximum is 3.5.
	}
	else
	{
		yield = 3.5 + ((prosperity - 200) / 30);
	}

	yield *= gameplaySettings->armyYieldMultiplier;

	return yield;
}

double LandTerritoryFeatures::calculateFleetYield() const
{
	// Gameplay settings must be known.
	assert(gameplaySettings != nullptr);

	double yield = 0;

	yield = 0.2 + (prosperity / 150);
	yield *= gameplaySettings->fleetYieldMultiplier;

	return yield;
}

sf::Color determineProsperityColor(double prosperity)
{
	const double yellowProsperity = 120;
	const double greenProsperity = 200; 
	const double darkGreenProsperity = 300;
	if(prosperity < yellowProsperity)
	{
		int green = 255 * (prosperity / yellowProsperity);
		return sf::Color(255, green, 0);
	}
	else if(prosperity <= greenProsperity)
	{
		int red = 255 * (1 - ((prosperity - yellowProsperity) / (greenProsperity - yellowProsperity)));
		return sf::Color(red, 255, 0);
	}
	else
	{
		int green = 255 - (155 * (prosperity - greenProsperity) / (darkGreenProsperity - greenProsperity));
		return sf::Color(0, green, 0);
	}

}
