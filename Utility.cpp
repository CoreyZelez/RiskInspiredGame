#include "Utility.h"
#include <sstream>
#include <iomanip>

sf::Color createRandomColor()
{
	// Seed the random number generator.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(50, 255); 

	// Generate random color components.
	sf::Uint8 red = static_cast<sf::Uint8>(dist(mt));
	sf::Uint8 green = static_cast<sf::Uint8>(dist(mt));
	sf::Uint8 blue = static_cast<sf::Uint8>(dist(mt));

	return sf::Color(red, green, blue);
}

sf::Color createRandomLandColor()
{
	// Seed the random number generator.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist1(180, 255); 
	std::uniform_int_distribution<int> dist2(50, 255); 

	// Generate random color components.
	sf::Uint8 red = 0;
	sf::Uint8 green = dist1(mt);
	sf::Uint8 blue = 0;
	if(dist1(mt) % 2 == 0)
	{
		blue = dist2(mt) / 4;
		red = dist2(mt);
	}
	else
	{
		blue = dist2(mt) / 2;
		red = dist2(mt) / 2;
	}

	return sf::Color(red, green, blue);
}

sf::Color createRandomNavalColor()
{
	// Seed the random number generator.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist1(180, 255);
	std::uniform_int_distribution<int> dist2(50, 255);

	// Generate random color components.
	sf::Uint8 red = 0;
	sf::Uint8 green = 0;
	sf::Uint8 blue = dist1(mt);
	if(dist1(mt) % 2 == 0)
	{
		green = dist2(mt) / 2;
		red = dist2(mt) / 2;
	}
	else
	{
		green = dist2(mt) / 4;
		red = dist2(mt);
	}

	return sf::Color(red, green, blue);
}

sf::Color createRandomRealmColor()
{
	// Seed the random number generator.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 200);
	std::uniform_int_distribution<int> distCombination(1, 6);

	// Generate random color components.
	sf::Uint8 red = 0;
	sf::Uint8 green = 0;
	sf::Uint8 blue = 0;

	int combinationVal = distCombination(mt);
	if(combinationVal == 1)
	{
		red = 200;
		green = dist(mt);
		blue = 40;
	}
	else if(combinationVal == 2)
	{
		red = 200;
		blue = dist(mt);
		green = 40;
	}
	else if(combinationVal == 3)
	{
		green = 200;
		red = dist(mt);
		blue = 40;
	}
	else if(combinationVal == 4)
	{
		green = 200;
		blue = dist(mt);
		red = 40;
	}
	else if(combinationVal == 5)
	{
		blue = 200;
		green = dist(mt);
		red = 40;
	}
	else if(combinationVal == 6)
	{
		blue = 200;
		red = dist(mt);
		green = 40;
	}

	return sf::Color(red, green, blue);
}

std::string generateRandomEstateName(Title title)
{
	return std::string();
}

std::string doubleToStringWithDecimals(double value, int decimals)
{
	std::ostringstream stream;

	if(decimals == 0) {
		// If 0 decimals, format without decimal point
		stream << std::fixed << std::setprecision(0) << value;
	}
	else {
		// Set the desired number of decimal places
		stream << std::fixed << std::setprecision(decimals) << value;
	}

	std::string result = stream.str();

	// If no decimals specified, remove decimal point
	if(decimals == 0) {
		size_t pos = result.find('.');
		if(pos != std::string::npos) {
			result.erase(pos, 1);
		}
	}

	return result;
}
