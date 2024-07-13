#pragma once
#include "Title.h"
#include "EditorGrid.h"
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
	~PlayerEstateManager();

	// Clears ownership of maridoms for when ruler game over. Cannot own non maridom estates.
	void clearAllMaridomOwnership();

	void handleMilitaryYields();

	void addEstate(Estate &estate);
	void removeEstate(const Estate &estate);

	std::unordered_set<const Estate*> getEstates() const;
	bool containsEstate(const Estate& estate) const;

	// Calculates army soft cap contributed by ruler owned estates.
	int calculateArmySoftCapContribution() const;
	// Calculates fleet soft cap contributed by ruler owned estates.
	int calculateFleetSoftCapContribution() const;

	// Ammends ownership of rulers unlanded estates.
	void ammendUnlandedEstateOwnership();

	// Returns true if any of landed estates in estates contains the position.
	bool landedEstatesContainsPosition(const sf::Vector2f &position) const;

	// Return title counts of directly controlled (non-vassal) estates.
	std::map<Title, int> getTitleCounts() const;
	// Returns highest estate title in estates.
	Title getHighestTitle() const;

private:
	std::unordered_set<Estate*> estates;  // Player owned non-vassal estates.
};

