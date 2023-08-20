#pragma once
#include "Title.h"
#include "Grid.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;
class PlayerMilitaryManager;

class Estate : public std::enable_shared_from_this<Estate>
{
public:
	Estate(Title title);
	Estate(Title title, const Grid &grid);

	void initRuler(Player &ruler);

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield(PlayerMilitaryManager &militaryManager);
	// Provides bonus yields to subfiefs. Yield dependant on title.
	virtual void provideSubfiefBonusYields();

	void addSubfief(std::shared_ptr<Estate> subfief);
	void removeSubfief(std::shared_ptr<Estate> &subfief);
	bool gridContainsPosition(const sf::Vector2f &position) const;
	void drawGrid(sf::RenderWindow &window) const;
	void setGridColor(sf::Color color);
	void setParent(const std::shared_ptr<const Estate> &parent);
	bool hasParent() const;
	bool compareRuler(const Player *player) const;  // For debugging.
	Title getTitle() const;

protected:
	// Military generating subfiefs receive specified bonus yield.
	virtual void receiveBonusYield(const float &bonus);
	// Estate attempts to create military units on map.
	virtual void generateMilitary(PlayerMilitaryManager &militaryManager); 	

	virtual bool containsPosition(const sf::Vector2f &position) const;
	Player* getRuler();
	void setRuler(Player *ruler);

private:
	Grid grid;
	const Title title;
	Player *ruler = nullptr;
	std::shared_ptr<const Estate> parent;
	std::vector<std::shared_ptr<Estate>> subfiefs;
};

