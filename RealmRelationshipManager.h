#pragma once
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Player;
class Estate;

class RealmRelationshipManager
{
public:
	RealmRelationshipManager(Player &player);

	void drawVassalRealms(sf::RenderWindow &window) const;

	// void receiveVassal
	// void transferVassal

	// Adds player as a vassal. Requires player has no liege.
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
	Player *liege;
	std::vector<Player*> vassals;
};

