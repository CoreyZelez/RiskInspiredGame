#pragma once
#include "MilitaryManager.h"
#include "Realm.h"
#include "PlayerAIComponent.h"
#include "LiegePolicy.h"
#include "VassalPolicy.h"
#include "PlayerDiplomacy.h"
#include <vector>
#include <memory>

class Game;

class Player : public HasUI
{
public:
	// Smart AI player.
	Player(Game &game, AIPersonality personality, const std::string &realmName);
	// Simple AI player.
	Player(Game &game, const std::string &realmName);

	virtual std::unique_ptr<UIEntity> createUI(UIType type) override;

	bool gameOver() const;

	void handleTurn();

	// Causes vassal player to rebel against liege. Can only rebel given certain conditions met and liege is liegeless.
	// In future consider implementing for case where liege has a liege. In which case ownership of vassal would be
	// transferred to the lieges liege. 
	void rebel();

	// Handles distribution of army reserves to player and their liege. Recurses to upper-most liege.
	void handleReserveArmyYield(double amount);
	// Handles distribution of fleet reserves to player and their liege. Recurses to upper-most liege.
	void handleReserveFleetYield(double amount);

	// Adds attacking player to attackHistory for player diplomacy.
	void addAttackHistory(Player &enemy);
	// Removes all diplomatic relations recorded with specified player.
	void removeDiplomacyWithPlayer(Player &player);
	// Sets colors of player realms with diplomacy relation to this player.
	void setDiplomacyColors(const std::vector<std::unique_ptr<Player>> &players);

	// Returns true if realm owner is a vassal of player. Optionally specify direct vassal only.
	bool isVassal(const Player &player, bool direct = true) const;
	// Returns true if player belongs to same upper realm as this->player.
	bool sameUpperLiege(const Player &player) const;

	// Returns the ruler of the upper most realm which player belongs to.
	Player& getUpperLiege();
	// Returns the ruler of the upper most realm which player belongs to.
	const Player& getUpperLiege() const;
	// Returns true if liege is not nullptr.
	bool hasLiege() const;
	// Returns liege.
	const Player* getLiege() const;
	// Returns liege.
	Player* getLiege();
	// Sets liege.
	void setLiege(Player *player);

	void setHuman();
	bool getIsHuman() const;

	MilitaryManager& getMilitaryManager();
	const MilitaryManager& getMilitaryManager() const;
	Realm& getRealm();
	const Realm& getRealm() const;
	const LiegePolicy &getLiegePolicy() const;
	const VassalPolicy &getVassalPolicy() const;

private:
	Game &game;
	Player *liege = nullptr;  // Liege of player.
	std::unique_ptr<PlayerAIComponent> AIComponent;
	LiegePolicy liegePolicy;
	VassalPolicy vassalPolicy;
	MilitaryManager militaryManager;
	PlayerDiplomacy diplomacy;
	Realm realm;
	bool isHuman = false;

	// Removes player as vassal.
	void handleVassalRebellion(Player &vassal);
	// Yields player reserves to a proportion of estates.
	void yieldArmyReserves();

};

// Returns true if players share the same upper realm i.e. are friendly.
// Returns false if either player is nullptr.
bool sameUpperRealm(const Player *player1, const Player *player2);