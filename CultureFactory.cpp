#include "CultureFactory.h"
#include "Culture.h"

CultureFactory::CultureFactory()
{
	Culture none = { -1, "none", sf::Color(60, 60, 60) };
	Culture temp = { 0, "temp", sf::Color(200, 30, 60) };
	add(none.id, none);
	add(temp.id, temp);
}

