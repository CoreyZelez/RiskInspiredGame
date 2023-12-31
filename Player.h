#pragma once
#include "MilitaryManager.h"
#include "Realm.h"
#include "PlayerAIComponent.h"
#include "LiegePolicy.h"
#include "VassalPolicy.h"
#include <vector>
#include <memory>

class Game;

class Player
{
public:
	// Smart AI player.
	Player(Game &game, AIPersonality personality);
	// Simple AI player.
	explicit Player(Game &game);

	void handleTurn();

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

private:
	Game &game;
	Player *liege = nullptr;  // Liege of player.
	std::unique_ptr<PlayerAIComponent> AIComponent;
	LiegePolicy liegePolicy;
	VassalPolicy vassalPolicy;
	MilitaryManager militaryManager;
	Realm realm;
	bool isHuman = false;
};

// Returns true if players share the same upper realm i.e. are friendly.
// Returns false if either player is nullptr.
bool sameRealm(const Player *player1, const Player *player2);