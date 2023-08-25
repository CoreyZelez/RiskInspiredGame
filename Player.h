#pragma once
#include "MilitaryManager.h"
#include "Realm.h"
#include "AIPersonality.h"
#include <vector>
#include <memory>

class Estate;
class Game;

class Player
{
public:
	// Human controlled player.
	explicit Player(Game &game);

	virtual void handleTurn();

	bool getAwaitingUserInput() const;
	void completeTurn();  // Call when human player ends turn through user input.

	MilitaryManager& getMilitaryMangager();
	Realm& getRealm();
	const Realm& getRealm() const;

protected:
	// getInformation()  for AI decision making.

private:
	Game &game;
	MilitaryManager militaryManager;
	Realm realm;
	bool awaitingUserInput = false;

	/*
	SHOULD HAVE AN INFORMATION CLASS HERE THAT STORES INFORMATION ABOUT NEIGHBOURING PLAYERS SUCH
	AS THEIR ARMY STRENGTH, THE DISTANCE OF THEIR ARMIES AND ALSO INFO ABOUT PLAYER. BASICALLY ALL INFO FOR AI DECISION MAKING.

	*/
};