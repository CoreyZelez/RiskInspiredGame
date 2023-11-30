#include "Port.h"
#include "TextureManager.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include <assert.h>

Port::Port(const LandTerritory &landTerritory, NavalTerritory &navalTerritory)
	: navalTerritory(navalTerritory), graphics(*(TextureManager::getInstance().getTexture("anchor")))
{
	sf::Vector2f spritePosition = determineSpritePosition(landTerritory, navalTerritory);
	graphics.setPosition(spritePosition);
}

int Port::getNavalTerritoryID() const
{
	return navalTerritory.getID();
}

sf::Vector2f Port::determineSpritePosition(const LandTerritory &landTerritory, const NavalTerritory &navalTerritory)
{
	// Territories must border each other.
	assert(landTerritory.getGrid().sharesBorder(navalTerritory.getGrid()));

	// Positions within land territory that border naval territory.
	// The sprite of the port will lie on one of these positions.
	std::vector<sf::Vector2f> borderingPositions = landTerritory.getGrid().getNeighbouringBorderPositions(navalTerritory.getGrid());

	// Calculate average position of border positions.
	sf::Vector2f averagePosition;
	for(sf::Vector2f position : borderingPositions)
	{
		averagePosition.x += position.x;
		averagePosition.y += position.y;
	}
	averagePosition.x /= borderingPositions.size();
	averagePosition.y /= borderingPositions.size();

	// Determine the closest bordering position to the average position of all bordering points.
	// This position is our choice for the sprite position.
	sf::Vector2f closestPosition = borderingPositions[0];
	float closestMetric = std::pow(std::abs(closestPosition.x - averagePosition.x), 2) + std::pow(std::abs(closestPosition.y - averagePosition.y), 2);
	for(sf::Vector2f position : borderingPositions)
	{
		const float metric = std::pow(std::abs(position.x - averagePosition.x), 2) + std::pow(std::abs(position.y - averagePosition.y), 2);
		if(metric < closestMetric)
		{
			closestPosition = position;
			closestMetric = metric;
		}
	}

	return closestPosition;
}
