#pragma once
#include "Title.h"
#include "Grid.h"
#include "HasUI.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Player;
class Estate;
class MilitaryManager;


class RealmEstateManager
{
public:
	RealmEstateManager(MilitaryManager &militaryManager);

	void draw(sf::RenderWindow &window) const;

	void handleFiefYields();

	void addFief(Estate *fief, bool updateGrid = true);
	void removeFief(const Estate *fief, bool updateGrid = true);

	// Returns true if realm grid contains specified world position.
	bool containsPosition(const sf::Vector2f &position) const;


	int getRealmSize() const;

private:
	MilitaryManager &militaryManager;
	std::vector<Estate*> fiefs;
	Grid grid;
};

