#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct Terrain
{
	std::string name;
	sf::Color color;
	double defenceMultiplier;
	double attackMultiplier;
};

