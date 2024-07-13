#include "EditorPort.h"
#include "TextureManager.h"
#include "EditorTerritory.h"
#include <assert.h>

EditorPort::EditorPort(const EditorTerritory& landTerritory, EditorTerritory& navalTerritory)
	: navalTerritory(navalTerritory), graphics(*(TextureManager::getInstance().getTexture("anchor")))
{
	assert(navalTerritory.getType() == TerritoryType::naval);
	assert(landTerritory.getType() == TerritoryType::land);

	sf::Vector2f spritePosition = determineSpritePosition(landTerritory, navalTerritory);
	graphics.setPosition(spritePosition);
	graphics.setScale(sf::Vector2f(3.5, 3.5));
}

void EditorPort::draw(sf::RenderWindow& window) const
{
	graphics.draw(window);
}

const EditorTerritory& EditorPort::getNavalTerritory() const
{
	return navalTerritory;
}

int EditorPort::getNavalTerritoryID() const
{
	return navalTerritory.getID();
}

sf::Vector2f EditorPort::determineSpritePosition(const EditorTerritory& landTerritory, const EditorTerritory& navalTerritory)
{
	// Territories must border each other.
	assert(landTerritory.getGrid().sharesBorder(navalTerritory.getGrid()));

	// Positions within land territory that border naval territory.
	// The sprite of the port will lie on one of these positions.
	std::unordered_set<sf::Vector2f, Vector2fHash> borderingPositions = landTerritory.getGrid().getAdjacentBorderPositions(navalTerritory.getGrid());

	// Calculate average position of border positions.
	sf::Vector2f averagePosition;
	for (const sf::Vector2f& position : borderingPositions)
	{
		averagePosition.x += position.x;
		averagePosition.y += position.y;
	}
	averagePosition.x /= borderingPositions.size();
	averagePosition.y /= borderingPositions.size();

	// Determine the closest bordering position to the average position of all bordering points.
	// This position is our choice for the sprite position.
	sf::Vector2f closestPosition = *borderingPositions.begin();
	float closestMetric = std::pow(std::abs(closestPosition.x - averagePosition.x), 2) + std::pow(std::abs(closestPosition.y - averagePosition.y), 2);
	for (sf::Vector2f position : borderingPositions)
	{
		const float metric = std::pow(std::abs(position.x - averagePosition.x), 2) + std::pow(std::abs(position.y - averagePosition.y), 2);
		if (metric < closestMetric)
		{
			closestPosition = position;
			closestMetric = metric;
		}
	}

	return closestPosition;
}
