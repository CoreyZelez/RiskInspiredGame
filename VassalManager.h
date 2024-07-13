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
	~VassalManager();

	// Set vassals as game over. Must have liege game over.
	void setVassalsGameOver();

	void drawVassalRealms(sf::RenderWindow &window) const;

	// void transferVassal
	// void addVassal(Player &player);
	// Creates a new player vassal and confers them the barony. Returns created player.
	Player& createVassal(Barony &barony);

	// Removes specified vassal from vassals. Vassal must not have any estates.
	void removeEstatelessVassal(const Player &vassal);
	void removeRebellingVassal(Player &vassal); // This is needed for when vassals rebel.

	// Confers estate to vassal. Returns the vassal player (direct or indirect) which becomes the estates direct owner.
	Player& conferEstate(Player &vassal, Estate &estate);
	
	// Removes estate from estates and updates territories if necessary.
	// Assumes estate already removed from vassal's realms.
	void removeEstate(const Estate& estate);

	// Ammends ownership of vassals unlanded estates.
	void ammendUnlandedEstateOwnership();

	// Calculates army soft cap contributed by vassals.
	int calculateArmySoftCapContribution(double contributionRatio) const;
	// Calculates fleet soft cap contributed by vassals.
	int calculateFleetSoftCapContribution(double contributionRatio) const;

	// Returns sum of all vassal's army reserves.
	int getTotalArmyReserves() const;

	const std::vector<Player*> &getVassals() const;

	std::unordered_set<const Estate*> getEstates() const;
	bool containsEstate(const Estate& estate) const;

	// Returns title counts of vassal controlled estates.
	std::map<Title, int> getTitleCounts() const;

private:
	Game &game;
	Player &ruler;
	std::vector<Player*> vassals;  // Direct vassals of ruler.
	std::unordered_set<Estate*> estates;  // All estates held by both direct vassals and indirect vassals.

	void addEstate(Estate *estate); 
};

