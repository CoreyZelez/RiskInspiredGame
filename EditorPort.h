#pragma once
#include "SpriteGraphicsComponent.h"

class EditorTerritory;

class EditorPort
{
public:
	EditorPort(const EditorTerritory& landTerritory, EditorTerritory& navalTerritory);

	void draw(sf::RenderWindow& window) const;

	const EditorTerritory& getNavalTerritory() const;

	int getNavalTerritoryID() const;

private:
	SpriteGraphicsComponent graphics;
	const EditorTerritory& navalTerritory;

	sf::Vector2f determineSpritePosition(const EditorTerritory& landTerritory, const EditorTerritory& navalTerritory);
};

