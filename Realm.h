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

	void addFief(Estate *fief, bool updateGrid = true);
	void removeFief(const Estate *fief, bool updateGrid = true);

	void addVassal(Player &vassal);
	// Returns true if realm owner is a indirect or direct vassal of player 
	bool isVassal(const Player &player, bool direct = true) const;
	// Returns true if player belongs to same upper realm as this->player.
	bool sameUpperRealm(const Player &player) const;
	// Returns the ruler of the upper most realm which player belongs to.
	Player& getUpperRealmRuler();
	// Returns the ruler of the upper most realm which player belongs to.
	const Player& getUpperRealmRuler() const;
	bool hasLiege() const;

private:


	Player &player;
	MilitaryManager &militaryManager;

	Player *liege;
	Title title;  // Official title of player.
	std::vector<Player*> vassals;
	std::vector<Estate*> fiefs;

	Grid grid;
	bool drawVassalRealms = false;  // Specifies to draw realms of vassals instead of realm of player.
};