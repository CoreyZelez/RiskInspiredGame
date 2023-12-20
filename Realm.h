#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include "PlayerEstateManager.h"
#include "VassalManager.h"
#include "RealmGrid.h"
#include <vector>

class Player;
class Estate;

class Realm : public HasUI
{
public:
	explicit Realm(Player &ruler);

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIEntity> getUI(UIType type) const override;

	// Handles yields of ruler estates.
	void handleMilitaryYields();

	// Returns true if realm owner is a indirect or direct vassal of player.
	bool isVassal(const Player &player, bool direct = true) const;
	// Returns true if player belongs to same upper realm as this->player.
	bool sameUpperRealm(const Player &player) const;
	// Returns the ruler of the upper most realm which player belongs to.
	Player& getUpperRealmRuler();
	// Returns the ruler of the upper most realm which player belongs to.
	const Player& getUpperRealmRuler() const;

	// Adds estate to realm, conferring to ruler or a vassal.
	void addEstate(Estate *estate);
	// Adds estate from realm, revoking from either ruler or a vassal depending on who owns it.
	void removeEstate(Estate *estate);

	////////////
	// Returns unordered set of all territories in realm, including those associated with both ruler owned and vassal estates.
	//
	//
	// CONSIDER HAVING A TERRITORY UNORDERED SET IN REALM CLASS DIRECTLY.
	// MUST CHECK PERFORMANCE BEFORE DETERMINING IF THIS IS NECESSARY.
	// WOULD REQUIRE UPDATING TERRITORIES IN REALM EVERY TIME CHANGES OCCUR TO BOTH
	std::unordered_set<Territory*> getTerritories();


	void updateGrid();

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;

	void setGridColor(const sf::Color &color);
	void setGridColorDefault();

	bool hasLiege() const;
	void setLiege(Player *player);

private:
	Player &ruler;  // Ruler of this realm.
	Player *liege;  // Liege of ruler of this realm.
	PlayerEstateManager rulerEstateManager;  // Manages estates directly controlled by ruler.
	VassalManager vassalManager;  // Manages vassals and their estates.
	RealmGrid realmGrid;  // Grid of entire realm estates.
	bool drawVassalRealms = false;  // Specifies to draw realms of vassals over entire realm grid.

	// Returns combined title counts of ruler estates and vassal estates.
	std::map<Title, int> getTitleCounts() const;
};