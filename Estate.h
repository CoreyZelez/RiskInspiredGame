#pragma once
#include "Title.h"
#include "Grid.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;
class MilitaryManager;

const std::string estateSaveLabel = "# estate";  // Save label for non landed estate.
const std::string baronySaveLabel = "# barony";  // Save label for barony.

class Estate
{
public:
	Estate(Title title);
	Estate(Title title, const Grid &grid);
	Estate(Title title, const Grid &grid, std::string name);

	void initName(std::string name);

	virtual void saveToFile(std::ofstream &file) const;

	void initRuler(Player &ruler);

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield(MilitaryManager &militaryManager);
	// Provides bonus yields to subfiefs. Yield dependant on title.
	virtual void provideSubfiefBonusYields();

	void addSubfief(Estate *subfief);
	void removeSubfief(Estate *subfief);
	void setParent(const Estate *parent);
	bool hasParent() const;
	bool compareRuler(const Player *player) const;  // For debugging.
	Title getTitle() const;
	std::string getName() const;

	Grid& getGrid();

protected:
	void initColor();

	// Military generating subfiefs receive specified bonus yield.
	virtual void receiveBonusYield(const float &bonus);
	// Estate attempts to create military units on map.
	virtual void generateMilitary(MilitaryManager &militaryManager);
	virtual std::string getSaveLabel() const;

	virtual bool containsPosition(const sf::Vector2f &position) const;
	Player* getRuler();
	void setRuler(Player *ruler);

private:
	void saveSubfiefs(std::ofstream &file) const;

	Grid grid;
	const Title title;
	Player *ruler = nullptr;
	const Estate *parent;
	std::vector<Estate*> subfiefs;
	std::string name = "";
};

