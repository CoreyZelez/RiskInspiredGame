#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include "RealmEstateManager.h"
#include "RealmRelationshipManager.h"
#include <vector>

class Player;
class Estate;
class MilitaryManager;

class Realm : public HasUI
{
public:
	explicit Realm(Player &player);

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIPanel> getUI(UIType type) const override;

	RealmEstateManager &getEstateManager();
	const RealmEstateManager &getEstateManager() const;
	RealmRelationshipManager &getRelationshipManager();
	const RealmRelationshipManager &getRelationshipManager() const;

private:
	Player &player;
	RealmEstateManager estateManager;
	RealmRelationshipManager relationshipManager;
	bool drawVassalRealms = false;  // Specifies to draw realms of vassals instead of realm of player.
};