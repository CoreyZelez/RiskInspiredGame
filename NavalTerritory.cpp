#include "NavalTerritory.h"
#include "ColorUtility.h"
#include "NavalTerritoryOccupancy.h"
#include <assert.h>
#include <fstream>



NavalTerritory::NavalTerritory(int id, EditorGrid grid)
	: Territory(TerritoryType::naval, id, grid, std::make_unique<NavalTerritoryOccupancy>(*this))
{
}

NavalTerritory::NavalTerritory(int id)
	: Territory(TerritoryType::naval, id, generateRandomNavalColor(), std::make_unique<NavalTerritoryOccupancy>(*this))
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
