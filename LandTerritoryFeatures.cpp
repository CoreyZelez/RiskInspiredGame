#include "LandTerritoryFeatures.h"

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
