#pragma once
#include <SFML/Graphics.hpp>

sf::Color generateRandomColor();

sf::Color generateRandomLandColor();

sf::Color generateRandomNavalColor();

sf::Color generateRandomRealmColor();

sf::Color getDarkenedColor(const sf::Color& color, double darkeningFactor);