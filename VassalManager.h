#pragma once
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_set>

class Player;
class Estate;
class Territory;
class RealmGrid;
class Barony;
class Game;
enum class Title;

class VassalManager
{
public:
	VassalManager(Game &game, Player &ruler);

	void drawVassalRealms(sf::RenderWindow &window) const;

	// void transferVassal
	// void addVassal(Player &player);
	// void removeVassal // This is needed for when vassals rebel.
	// Creates a new player vassal and confers them the barony. Returns created player.
	Player& createVassal(Barony &barony);

	// Confers estate to vassal. Returns the vassal player (direct or indirect) which becomes the estates direct owner.
	Player& conferEstate(Player &vassal, Estate &estate);
	
	// Removes estate from estates and updates territories if necessary.
	// Assumes estate already removed from vassal's realms.
	void removeEstate(Estate& estate);

	const std::vector<Player*> &getVassals() const;

	const std::unordered_set<Territory*> &getTerritories();

	std::unordered_set<const Estate*> getEstates() const;
	bool containsEstate(const Estate& estate) const;

	// Returns title counts of vassal controlled estates.
	std::map<Title, int> getTitleCounts() const;

private:
	Game &game;
	Player &ruler;
	std::vector<Player*> vassals;  // Direct vassals of ruler.
	std::unordered_set<Estate*> estates;  // All estates held by both direct vassals and indirect vassals.
	std::unordered_set<Territory*> territories;  // Territories spanned by all vassal estates.

	void addEstate(Estate *estate); 
};

