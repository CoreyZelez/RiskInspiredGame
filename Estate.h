#pragma once
#include "Title.h"
#include "Grid.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;
class LandArmy;
class NavalFleet;


class Estate : public std::enable_shared_from_this<Estate>
{
public:
	Estate(Title title, const Player *ruler);
	Estate(Title title);
	Estate(Title title, const Player *ruler, const Grid &grid);
	Estate(Title title, const Grid &grid);

	void addSubFief(std::shared_ptr<Estate> subFief);
	void removeSubFief(std::shared_ptr<Estate> &subFief);

	bool gridContainsPosition(const sf::Vector2f &position) const;
	void drawGrid(sf::RenderWindow &window) const;
	void setGridColor(sf::Color color);

	void setParent(const std::shared_ptr<const Estate> &parent);
	bool hasParent() const;
	Title getTitle() const;

protected:
	virtual bool containsPosition(const sf::Vector2f &position) const;
	const Player* getRuler() const;
	void setRuler(const Player *ruler);

private:
	Grid grid;
	const Title title;
	const Player *ruler = nullptr;
	std::shared_ptr<const Estate> parent;
	std::vector<std::shared_ptr<Estate>> subFiefs;
};

