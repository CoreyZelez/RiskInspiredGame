#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>

class Player;
class Estate;
class MilitaryManager;
class Territory;
class RealmGrid;

class PlayerEstateManager
{
public:
	PlayerEstateManager(RealmGrid &realmGrid, MilitaryManager &militaryManager);

	void handleMilitaryYields();

	void addEstate(Estate *estate);
	void removeEstate(Estate *estate);

	const std::unordered_set<Territory*> &getTerritories();

	// Return title counts of directly controlled (non-vassal) estates.
	std::map<Title, int> getTitleCounts() const;

private:
	MilitaryManager &militaryManager;
	RealmGrid &realmGrid;
	std::vector<Estate*> estates;  // Player owned non-vassal estates.
	std::unordered_set<Territory*> territories;  // Territories spanned by all ruler owned (non-vassal) estates.
};

