#pragma once
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_set>

class Player;
class Estate;
class Territory;
class RealmGrid;
enum class Title;

class VassalManager
{
public:
	VassalManager(Player &ruler, RealmGrid &realmGrid);

	void drawVassalRealms(sf::RenderWindow &window) const;

	// void receiveVassal
	// void transferVassal

	// void removeVassal // This is needed for when vassals rebel.

	// void createVassal(barony).

	//////
	// IS THIS FUNCTION USEFUL? USUALLY WE WANT TO CREATE A NEW PLAYER FOR OUR VASSALS NOT
	//
	// Adds player as a vassal. 
	void addVassal(Player &player);
	//

	const std::vector<Player*> &getVassals() const;

	const std::unordered_set<Territory*> &getTerritories();

	// Returns title counts of vassal controlled estates.
	std::map<Title, int> getTitleCounts() const;

private:
	Player &ruler;
	RealmGrid &realmGrid;
	std::vector<Player*> vassals;  // Direct vassals of ruler.
	std::vector<Estate*> estates;  // All estates held by both direct vassals and indirect vassals.
	std::unordered_set<Territory*> territories;  // Territories spanned by all vassal estates.
};

