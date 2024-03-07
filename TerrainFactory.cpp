#include "TerrainFactory.h"
#include "Terrain.h"

TerrainFactory::TerrainFactory()
{
	Terrain temp = { -1, "temp", sf::Color(30, 30, 30), 0, 0 };
	Terrain plains = { 0, "plains", sf::Color(65, 195, 10), 1.1, 1 };
	Terrain forest = { 1, "forest", sf::Color(0, 150, 8), 0.9, 0.85 };
	Terrain hills = { 2, "hills", sf::Color(145, 100, 30), 1.1, 0.9 };
	Terrain mountains = { 3, "mountains", sf::Color(90, 85, 70), 1.2, 0.8 };
	Terrain desert = { 4, "desert", sf::Color(255, 225, 30), 1.3, 1.2 };
	Terrain scrubland = { 5, "scrubland", sf::Color(235, 175, 0), 1.2, 1.1 };
	Terrain tundra = { 6, "tundra", sf::Color(110, 150, 170), 1.2, 1.1 };
	Terrain snow = { 7, "snow", sf::Color(210, 210, 210), 1.4, 1.25 };

	add(temp.id, temp);
	add(plains.id, plains);
	add(forest.id, forest);
	add(hills.id, hills);
	add(mountains.id, mountains);
	add(desert.id, desert);
	add(scrubland.id, scrubland);
	add(tundra.id, tundra);
	add(snow.id, snow);
}
