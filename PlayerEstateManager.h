#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include <vector>
#include <unordered_set>
#include <SFML/Graphics.hpp>

class Player;
class Estate;
class MilitaryManager;
class Territory;

class PlayerEstateManager
{
public:
	void handleMilitaryYields();

	void addEstate(Estate &estate);
	void removeEstate(Estate &estate);

	const std::unordered_set<Territory*> &getTerritories();
	std::unordered_set<const Estate*> getEstates() const;
	bool containsEstate(const Estate& estate) const;

	// Returns true if any of landed estates in estates contains the position.
	bool landedEstatesContainsPosition(const sf::Vector2f &position) const;

	// Return title counts of directly controlled (non-vassal) estates.
	std::map<Title, int> getTitleCounts() const;
	// Returns highest estate title in estates.
	Title getHighestTitle() const;

private:
	std::unordered_set<Estate*> estates;  // Player owned non-vassal estates.
	std::unordered_set<Territory*> territories;  // Territories spanned by all ruler owned (non-vassal) estates.
};

