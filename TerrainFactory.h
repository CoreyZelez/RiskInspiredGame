#pragma once
#include "Terrain.h"
#include <string>

class TerrainFactory
{
public:
	TerrainFactory();

	Terrain createTerrain(int id) const;
	bool hasTerrain(int id) const;

private:
	std::map<int, Terrain> terrains;
};

