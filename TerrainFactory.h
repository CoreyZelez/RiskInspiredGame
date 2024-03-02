#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>

class Terrain;

class TerrainFactory : public Factory<int, Terrain>

{
public:
	TerrainFactory();
};