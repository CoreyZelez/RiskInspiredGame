#include "NavalTerritory.h"
#include "ColorUtility.h"
#include "NavalTerritoryOccupancy.h"
#include <assert.h>
#include <fstream>



NavalTerritory::NavalTerritory(int id, Grid grid)
	: Territory(TerritoryType::naval, id, grid, std::make_unique<NavalTerritoryOccupancy>(*this))
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
		// MAYBE HAVE TO DO SOMETHING HERE WITH BORDERS OR SOME SHIT.
		break;
	}
	
	getGrid().setInteriorColor(color);
}
