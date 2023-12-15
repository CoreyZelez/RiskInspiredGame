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

class RealmEstateManager
{
public:
	RealmEstateManager(MilitaryManager &militaryManager);

	void draw(sf::RenderWindow &window) const;

	void setGridColor(const sf::Color &color);
	void setGridColorDefault();

	void handleFiefYields();

	void addFief(Estate *fief, bool updateGrid = true);
	void removeFief(Estate *fief, bool updateGrid = true);

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;

	std::set<Territory*> &getTerritories();
	int getRealmSize() const;

	// Return title counts of directly controlled (non-vassal) estates.
	std::map<Title, int> getTitleCounts() const;

private:
	MilitaryManager &militaryManager;
	////////////////////////////////////////////////////////////
	// SHOULD PROBABLY CHANGE THIS TO SET FOR MORE EFFICIENCY!//
	std::vector<Estate*> fiefs;  ///////////////////////////////
	////////////////////////////////////////////////////////////
	std::set<Territory*> territories;
	const sf::Color defaultGridColor;
	Grid grid;
};

