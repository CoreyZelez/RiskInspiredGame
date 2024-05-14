#pragma once
#include "HasUI.h"
#include "Subject.h"
#include "Title.h"
#include "EditorGrid.h"
#include "PlayerEstateManager.h"
#include "VassalManager.h"
#include "RealmGrid.h"
#include <vector>
#include <unordered_map>

class Player;
class LiegePolicy;
class Estate;
class Game;
class MilitaryManager;

class Realm : public HasUI, public Subject
{
public:
	Realm(Game &game, Player &ruler, const LiegePolicy &liegePolicy, const std::string &name);

	void handleGameOver();

	void draw(sf::RenderWindow &window) const;

	virtual std::unique_ptr<UIEntity> createUI(UIType type) const override;

	// Returns the soft cap on the associated player armies.
	int calculateArmySoftCap() const;
	// Returns the soft cap on the associated player fleets.
	int calculateFleetSoftCap() const;
	// Returns percent of yielded armies from both vassals and personally owned estates that are retained.
	double getEffectiveArmyYieldRatio();
	// Returns percent of yielded fleets from both vassals and personally owned estates that are retained.
	double getEffectiveFleetYieldRatio(); 
	// Handles yields of ruler estates.
	void handleMilitaryYields();
	// Yields all army reserves to realm land territories.
	void yieldArmyReserves();

	// Removes rebelling vassal from realm.
	void removeRebellingVassal(Player &vassal);
	// Redetermines and updates the ownership of rulers and vassals unlanded estates.
	void ammendUnlandedEstateOwnership();

	// Adds estate to realm, conferring to ruler or a vassal. 
	// Returns the player which estate is conferred to for territory to.
	Player& addEstate(Estate &estate);
	// Removes estate from realm, revoking from either ruler or a vassal depending on who owns it.
	void removeEstate(Estate &estate);
	// Returns highest ruler title of any estate's title of ruler's personally held estates.
	Title getHighestRulerTitle() const;
	// Returns combined title counts of ruler estates and vassal estates.
	std::map<Title, int> getTitleCounts() const;

	// CONSIDER HOLDING SETS FOR THE BELOW TWO FUNCTIONS IN REALM CLASS SO CAN RETURN REFERENCE. FOR OPTIMISAT5ION REASONS!!!
	// Returns unordered set of all territories in realm, including those associated with both ruler owned and vassal estates.
	std::unordered_set<Territory*> getTerritories();
	// Returns unordered set of all estates in realm, including both ruler and vassal owned estates.
	std::unordered_set<const Estate*> getEstates() const;

	// Returns vassal view.
	bool getVassalView() const;

	bool hasNoBaronies() const;

	bool gridIsOutdated() const;

	std::string getName() const;

	// Returns sum of all vassal's army reserves.
	int getTotalVassalArmyReserves() const;

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
	RealmGrid realmGrid;  // EditorGrid of entire realm estates.
	bool vassalView = false;  // Specifies to draw realms of vassals over entire realm grid.
	double effectiveArmyYieldRatio;
	double effectiveFleetYieldRatio;
	bool effectiveArmyYieldRatioOutdated = true;
	bool effectiveFleetYieldRatioOutdated = true;

	// Confers the estate to ruler or a vassal. Returns the player estate is conferred to.
	// This function does not update the estates ownership. It should only be called by the 
	// estate in question so the estate itself can update its ownership.
	Player& allocate(Estate &estate);

	// Returns true if the barony is to be conferred to the ruler.
	bool shouldConferBaronyToRuler(Barony &barony) const;
	// Returns true if the barony is to be conferred to a newly created vassal.
	bool shouldConferBaronyToNewVassal(Barony &barony) const;

	// Returns the vassal with the highest varony conferral score. This is the vassal
	// which the barony should be conferred to.
	Player* getHighestBaronyConferralScoreVassal(const Barony &barony) const;

	// Returns positive conferral contribution score from vassals number of controlled baronies. 
	// Vassals are awarded contribution score for having relatively smaller realms.
	double realmSizeBaronyConferralContribution(const Player &vassal) const;
	// Returns conferral contribution from vassal owning related estates to barony.
	double realmEstatesInfluenceBaronyConferralContribution(const Player &vassal, const Barony &barony) const;

	// Yields all fleet reinforcements at realm ports.
	void yieldFleetReinforcements();

};

// Returns the player with the greatest influence over the specified barony from a parameter vector of players.
Player &getGreatestBaronyInfluence(const Barony &barony, const std::vector<Player*> &players);
// Returns the player's barony influence.
int calculateBaronyInfluence(const Barony &barony, const Player &player, std::unordered_map<Title, int> &influenceContributions);

// Returns the player with the greatest influence over the specified unlanded estate from a parameter vector of players.
Player &getGreatestUnlandedEstateInfluence(const Estate &estate, const std::vector<Player*> &players);

// Computes the number of unlanded estates from an estate count map.
int countUnlandedEstates(std::map<Title, int> &estateCounts);