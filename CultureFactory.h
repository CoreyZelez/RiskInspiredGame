#pragma once
#include "Culture.h"
#include <string>

class CultureFactory
{
public:
	CultureFactory();

	Culture createCulture(int id) const;
	bool hasCulture(int id) const;

private:
	std::map<int, Culture> cultures;
};

