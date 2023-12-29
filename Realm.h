#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include "PlayerEstateManager.h"
#include "VassalManager.h"
#include "RealmGrid.h"
#include "EstateAllocator.h"
#include <vector>

class Player;
class Estate;
class Game;

class Realm : public HasUI
{
public:
	Realm(Game &game, Player &ruler);

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIEntity> getUI(UIType type) const override;

	// Handles yields of ruler estates.
	void handleMilitaryYields();

	// Returns true if realm owner is a indirect or direct vassal of player. Optionally specify direct vassal only.
	bool isVassal(const Player &player, bool direct = true) const;
	// Returns true if player belongs to same upper realm as this->player.
	bool sameUpperRealm(const Player &player) const;
	// Returns the ruler of the upper most realm which player belongs to.
	Player& getUpperRealmRuler();
	// Returns the ruler of the upper most realm which player belongs to.
	const Player& getUpperRealmRuler() const;
	// Returns true if liege is not nullptr.
	bool hasLiege() const;
	// Returns liege.
	const Player* getLiege() const;
	// Sets liege.
	void setLiege(Player *player);

	// Adds estate to realm, conferring to ruler or a vassal. 
	// Returns the player which estate is conferred to for territory to.
	Player& addEstate(Estate &estate);
	// Removes estate from realm, revoking from either ruler or a vassal depending on who owns it.
	void removeEstate(Estate &estate);
	// Returns highest ruler title.
	Title getHighestRulerTitle() const;

	// CONSEIDER HOLDING SETS FOR THE BELOW TWO FUNCTIONS IN REALM CLASS SO CAN RETURN REFERENCE. FOR OPTIMISAT5ION REASONS!!!
	// Returns unordered set of all territories in realm, including those associated with both ruler owned and vassal estates.
	std::unordered_set<Territory*> getTerritories();
	// Returns unordered set of all estates in realm, including both ruler and vassal owned estates.
	std::unordered_set<const Estate*> getEstates() const;

	void updateGrid();

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;
	// Changes color of realms grid.
	void setGridColor(const sf::Color &color);
	// Sets color of realm grid to default color.
	void setGridColorDefault();

private:
	Player &ruler;  // Ruler of this realm.
	Player *liege = nullptr;  // Liege of ruler of this realm.
	PlayerEstateManager rulerEstateManager;  // Manages estates directly controlled by ruler.
	VassalManager vassalManager;  // Manages vassals and their estates.
	EstateAllocator estateAllocator;  // Allocates estates to either ruler or vassals.
	RealmGrid realmGrid;  // Grid of entire realm estates.
	bool drawVassalRealms = false;  // Specifies to draw realms of vassals over entire realm grid.

	// Returns combined title counts of ruler estates and vassal estates.
	std::map<Title, int> getTitleCounts() const;
};