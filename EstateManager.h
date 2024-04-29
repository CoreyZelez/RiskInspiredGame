#pragma once
#include "Title.h"
#include "Estate.h"
#include "LandedEstate.h"
#include "NameGenerator.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <map>

class LandTerritory;
class NavalTerritory;
class CoastalTerritory;
class TerritoryManager;

class EstateManager
{
public:
	EstateManager();
	void draw(sf::RenderWindow &window, Title title) const;  // Draws all estates with estates of title drawn over all other titles.
	void draw(sf::RenderWindow &window) const;  // Draws all estates without a parent estate.
	void drawUnownedMaridoms(sf::RenderWindow &window) const;

	void save(std::string mapName) const;

	void load(std::string mapName, std::vector<std::unique_ptr<LandTerritory>>& landTerritories, 
		std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories);

	// Adds and removes baronies as necessary dependant on land territories.
	void reconcileBaronies(const std::vector<std::unique_ptr<LandTerritory>> &landTerritories);

	// Adds and removes maridoms as necessary dependant on naval territories.
	void reconcileMaridoms(const std::vector<std::unique_ptr<NavalTerritory>> &navalTerritories);

	// Returns vector to all baronies.
	std::vector<std::unique_ptr<Estate>>& getBaronies();

	Estate* createEstate(Title title);  // Creates estate with title and returns handle.
	///void removeEstate(Estate *estate);  // Removes estate.

	// Returns pointer to estate at world position. Allows specifying parent.
	Estate* getEstate(sf::Vector2f position, Title title, bool allowParent);  

	// Returns pointer to estate at world position with associated title..
	const Estate* getEstate(sf::Vector2f position, Title title) const;  

	Estate* getLowerEstate(sf::Vector2f position, Title title, bool allowParent);  // Returns pointer to estate at world position.

	void makeColored(Title title, bool setLower);  // Makes estate grids colored for specified title(s). Other estates made grey.

private:
	NameGenerator nameGenerator;
	std::map<Title, std::vector<std::unique_ptr<Estate>>, TitleComparer> estates;
	std::unordered_set<int> allocatedTerritoryIDs;
	std::unordered_set<std::string> allocatedEstateNames;

	std::string generateName();

	Estate *getFief(std::string name);

	void loadBarony(std::ifstream &file, std::vector<std::unique_ptr<LandTerritory>>& landTerritories);
	void loadMaridom(std::ifstream &file, std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories);
	void loadEstate(std::ifstream &file);
	std::string loadName(std::ifstream &file);
	sf::Color loadColor(std::ifstream &file);
	std::vector<std::string> loadSubfiefNames(std::ifstream &file);

	void setTitleColor(Title title);  // Sets any estates grid with title to pre defined color.
	void setTitleColor(Title title, sf::Color color);  // Sets any estates grid with title to color.
};