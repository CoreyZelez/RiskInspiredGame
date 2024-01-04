#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include "PlayerEstateManager.h"
#include "VassalManager.h"
#include "RealmGrid.h"
#include <vector>

class Player;
class LiegePolicy;
class Estate;
class Game;

class Realm : public HasUI
{
public:
	Realm(Game &game, Player &ruler, const LiegePolicy &liegePolicy, const std::string &name);

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIEntity> getUI(UIType type) const override;

	// Handles yields of ruler estates.
	void handleMilitaryYields();

	// Adds estate to realm, conferring to ruler or a vassal. 
	// Returns the player which estate is conferred to for territory to.
	Player& addEstate(Estate &estate);
	// Removes estate from realm, revoking from either ruler or a vassal depending on who owns it.
	void removeEstate(Estate &estate);
	// Returns highest ruler title of any estate's title in realm.
	Title getHighestRulerTitle() const;

	// CONSIDER HOLDING SETS FOR THE BELOW TWO FUNCTIONS IN REALM CLASS SO CAN RETURN REFERENCE. FOR OPTIMISAT5ION REASONS!!!
	// Returns unordered set of all territories in realm, including those associated with both ruler owned and vassal estates.
	std::unordered_set<Territory*> getTerritories();
	// Returns unordered set of all estates in realm, including both ruler and vassal owned estates.
	std::unordered_set<const Estate*> getEstates() const;

	void updateGrid();

	// Returns true if realm contains specified world position. Can choose whether to consider vassalView boolean
	// which in the case that vassalView is true, the function will only return true if the position to the landed 
	// estates directly controlled by the ruler.
	bool containsPosition(const sf::Vector2f &position, bool considerVassalView = false) const;

	void setVassalView(bool vassalView);

	// Changes color of realms grid.
	void setGridColor(const sf::Color &color);
	// Sets color of realm grid to default color.
	void setGridColorDefault();

private:
	const std::string name;
	Player &ruler;  // Ruler of this realm.
	const LiegePolicy &liegePolicy;
	PlayerEstateManager rulerEstateManager;  // Manages estates directly controlled by ruler.
	VassalManager vassalManager;  // Manages vassals and their estates.
	RealmGrid realmGrid;  // Grid of entire realm estates.
	bool vassalView = false;  // Specifies to draw realms of vassals over entire realm grid.

	// Confers the estate to ruler or a vassal. Returns the player estate is conferred to.
	// This function does not update the estates ownership. It should only be called by the 
	// estate in question so the estate itself can update its ownership.
	Player& allocate(Estate &estate);

	// Returns combined title counts of ruler estates and vassal estates.
	std::map<Title, int> getTitleCounts() const;

	// Returns true if the barony must be conferred to the ruler.
	bool mustConferBaronyToRuler(Barony &barony) const;
};

// Returns the player with the greatest influence over the specified barony from a parameter vector of players.
Player &getGreatestBaronyInfluence(const Barony &barony, const std::vector<Player*> &players);

// Returns the player with the greatest influence over the specified unlanded estate from a parameter vector of players.
Player &getGreatestUnlandedEstateInfluence(const Estate &estate, const std::vector<Player*> &players);

// Computes the number of unlanded estates from an estate count map.
int countUnlandedEstates(std::map<Title, int> &estateCounts);