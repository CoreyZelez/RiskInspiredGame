#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>

struct Terrain;

class TerrainFactory : public Factory<int, Terrain>

{
public:
	TerrainFactory();
};