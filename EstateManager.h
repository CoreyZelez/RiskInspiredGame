#pragma once
#include "Title.h"
#include "Estate.h"
#include "LandedEstate.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <map>

class LandTerritory;
class CoastalTerritory;

class EstateManager
{
public:
	void draw(sf::RenderWindow &window, Title title) const;  // Draws all estates with estates of title drawn over all other titles.
	void draw(sf::RenderWindow &window) const;  // Draws all estates without a parent estate.

	void save(std::string mapName) const;
	void load(std::string mapName);

	// Adds and removes baronies as necessary dependant on land territories.
	void reconcileBaronies(const std::vector<std::unique_ptr<LandTerritory>> &landTerritories);

	Estate* createEstate(Title title);  // Creates estate with title and returns handle.
	void removeEstate(Estate *estate);  // Removes estate and nulls pointer handle.
	Estate* getEstate(sf::Vector2f position, Title title, bool allowParent);  // Returns pointer to estate at world position.
	Estate* getLowerEstate(sf::Vector2f position, Title title, bool allowParent);  // Returns pointer to estate at world position.

	void makeColored(Title title, bool setLower);  // Makes estate grids colored for specified title(s). Other estates made grey.

private:
	void setTitleColor(Title title, sf::Color color);  // Sets any estates grid with title to color.

	std::unordered_set<int> allocatedIDs;
	std::map<Title, std::vector<std::unique_ptr<Estate>>, TitleComparer> estates;
};

