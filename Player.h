#pragma once
#include "MilitaryManager.h"
#include "Realm.h"
#include "AIPlayerComponent.h"
#include <vector>
#include <memory>

class Estate;
class Game;

class Player
{
public:
	// AI controlled player.
	explicit Player(Game &game, AIPersonality personality);

	void handleTurn();

	void setHuman();
	bool getIsHuman() const;

	MilitaryManager& getMilitaryManager();
	const MilitaryManager& getMilitaryManager() const;
	Realm& getRealm();
	const Realm& getRealm() const;

	// getInformation()  for AI decision making.

private:
	Game &game;
	MilitaryManager militaryManager;
	Realm realm;
	AIPlayerComponent AIComponent;
	bool isHuman = false;
};