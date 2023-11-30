#pragma once
#include "SpriteGraphicsComponent.h"
#include <memory>
#include <SFML/Graphics.hpp>

class NavalTerritory;
class LandTerritory;

class Port
{
public:
	Port(const LandTerritory &landTerritory, NavalTerritory& navalTerritory);

	int getNavalTerritoryID() const;

private:
	SpriteGraphicsComponent graphics;
	NavalTerritory &navalTerritory;  

	sf::Vector2f determineSpritePosition(const LandTerritory &landTerritory, const NavalTerritory &navalTerritory);
};

