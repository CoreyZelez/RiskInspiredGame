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
