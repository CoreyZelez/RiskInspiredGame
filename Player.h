#pragma once
#include "Title.h"
#include "PlayerMilitaryManager.h"
#include "PlayerRelationshipManager.h"
#include "AIPlayerController.h"
#include <vector>
#include <memory>

class Estate;
class Game;
class IController;

class Player
{
public:
	// Human controlled player.
	Player(Game &game);
	// AI controlled player.
	Player(Game &game, PersonalityAI personality);

	void update();

	void handleFiefYields();

	void addFief(Estate *fief);
	void removeFief(const Estate *fief);

	IController &getController();

	PlayerRelationshipManager &getRelationshipManager();

private:
	Game &game;

	std::unique_ptr<IController> controller;

	Title title;  // Official title of player.
	std::vector<Estate*> fiefs;
	PlayerMilitaryManager militaryManager;
	PlayerRelationshipManager relationshipManager;
	/*
	SHOULD HAVE AN INFORMATION CLASS HERE THAT STORES INFORMATION ABOUT NEIGHBOURING PLAYERS SUCH
	AS THEIR ARMY STRENGTH, THE DISTANCE OF THEIR ARMIES 
	
	*/
};

