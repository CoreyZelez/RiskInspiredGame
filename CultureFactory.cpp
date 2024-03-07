#include "CultureFactory.h"
#include "Culture.h"

CultureFactory::CultureFactory()
{
	Culture temp = { -1, "temp", sf::Color(30, 30, 30) };
	Culture none = { 0, "none", sf::Color(60, 60, 60) };
	add(temp.id, temp);
	add(none.id, none);
}

