#include "NavalTerritory.h"
#include "Utility.h"
#include "NavalTerritoryOccupancy.h"
#include <assert.h>
#include <fstream>



NavalTerritory::NavalTerritory(int id, Grid grid)
	: Territory(id, grid, std::make_unique<NavalTerritoryOccupancy>(*this),  TerritoryType::naval)
{
}

NavalTerritory::NavalTerritory(int id)
	: Territory(id, createRandomNavalColor(), std::make_unique<NavalTerritoryOccupancy>(*this), TerritoryType::naval)
{
}

std::string NavalTerritory::getSaveLabel() const
{
	return navalSaveLabel;
}

void NavalTerritory::setDrawMode(TerritoryDrawMode mode)
{
	Territory::setDrawMode(mode);

	sf::Color color(120, 120, 120);

	switch(mode)
	{
	case TerritoryDrawMode::terrain:
		color = sf::Color(70, 70, 200);
		getGrid().setAllPositionsDark();
		break;
	}
	
	getGrid().setColor(color);
}

