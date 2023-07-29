#include "Utility.h"

sf::Color createRandomColor()
{
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(50, 255); // Random values between 0 and 255

	// Generate random color components
	sf::Uint8 red = static_cast<sf::Uint8>(dist(mt));
	sf::Uint8 green = static_cast<sf::Uint8>(dist(mt));
	sf::Uint8 blue = static_cast<sf::Uint8>(dist(mt));

	// Create a random color using SFML
	return sf::Color(red, green, blue);
}
