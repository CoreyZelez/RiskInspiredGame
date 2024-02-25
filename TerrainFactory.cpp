#include "TerrainFactory.h"

TerrainFactory::TerrainFactory()
{
	Terrain plains = { 0, "plains", sf::Color(65, 195, 10), 1, 1 };
	Terrain forest = { 1, "forest", sf::Color(0, 150, 8), 0.9, 0.8 };
	Terrain hills = { 2, "hills", sf::Color(145, 100, 30), 1.1, 0.9 };
	Terrain mountains = { 3, "mountains", sf::Color(90, 85, 70), 1.2, 0.8 };
	Terrain desert = { 4, "desert", sf::Color(255, 225, 30), 1.3, 1.2 };
	Terrain scrubland = { 5, "scrubland", sf::Color(235, 175, 0), 1.1, 1.1 };
	terrains[plains.id] = plains;
	terrains[forest.id] = forest;
	terrains[hills.id] = hills;
	terrains[mountains.id] = mountains;
	terrains[desert.id] = desert;
	terrains[scrubland.id] = scrubland;
}

bool TerrainFactory::hasTerrain(int id) const
{
	return terrains.count(id);
}

Terrain TerrainFactory::createTerrain(int id) const
{
	return terrains.at(id);
}
