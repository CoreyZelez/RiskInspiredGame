#pragma once
#include "Title.h"
#include "Grid.h"
#include <vector>

class Player;
class Estate;
class MilitaryManager;

class Realm
{
public:
	explicit Realm(Player &player);

	void draw(sf::RenderWindow &window) const;

	// void receiveVassal
	// void transferVassal
	// Adds player as a vassal. Requires player has no liege.

	void handleFiefYields();

	void addFief(Estate *fief);
	void removeFief(const Estate *fief);

	void addVassal(Player &vassal);
	bool isVassal(const Player &player) const;
	bool hasLiege() const;

private:
	const Player &player;
	MilitaryManager &militaryManager;

	const Player *liege;
	Title title;  // Official title of player.
	std::vector<Player*> vassals;
	std::vector<Estate*> fiefs;

	Grid grid;
	bool drawVassalRealms = false;  // Specifies to draw realms of vassals instead of realm of player.
};