#include "CultureFactory.h"

CultureFactory::CultureFactory()
{
	Culture none = { -1, "none", sf::Color(60, 60, 60) };
	Culture temp = { 0, "temp", sf::Color(200, 30, 60) };
	cultures[none.id] = none;
	cultures[temp.id] = temp;
}

Culture CultureFactory::createCulture(int id) const
{
	return cultures.at(id);
}

bool CultureFactory::hasCulture(int id) const
{
	return cultures.count(id);
}
