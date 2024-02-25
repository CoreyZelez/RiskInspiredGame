#include "CultureFactory.h"

CultureFactory::CultureFactory()
{
	Culture none = { -1, "none" };
	cultures[none.id] = none;
}

Culture CultureFactory::createCulture(int id) const
{
	return cultures.at(id);
}

bool CultureFactory::hasCulture(int id) const
{
	return cultures.count(id);
}
