#pragma once
#include "MilitaryManager.h"
#include "Realm.h"
#include "PlayerAIComponent.h"
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

	void setHuman();
	bool getIsHuman() const;

	MilitaryManager& getMilitaryManager();
	const MilitaryManager& getMilitaryManager() const;
	Realm& getRealm();
	const Realm& getRealm() const;

private:
	Game &game;
	std::unique_ptr<PlayerAIComponent> AIComponent;
	MilitaryManager militaryManager;
	Realm realm;
	bool isHuman = false;
};