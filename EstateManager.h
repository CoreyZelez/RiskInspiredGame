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
class NavalTerritory;
class CoastalTerritory;
class TerritoryManager;

class EstateManager
{
public:
	EstateManager();

	// Draws all estates with estates of title drawn over all other titles.
	void draw(sf::RenderWindow &window, Title title) const;  
	// Draws all estates without a parent estate.
	void draw(sf::RenderWindow &window) const;  
	void drawUnownedMaridoms(sf::RenderWindow &window) const;

	void load(std::string mapName, std::vector<std::unique_ptr<LandTerritory>>& landTerritories, 
		std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories);

	// Returns vector to all baronies.
	std::vector<std::unique_ptr<Estate>>& getBaronies();

	// Returns pointer to estate at world position. Allows specifying parent.
	Estate* getEstate(sf::Vector2f position, Title title, bool allowParent);  
	// Returns pointer to estate at world position with associated title.
	const Estate* getEstate(sf::Vector2f position, Title title) const;  

private:
	std::map<Title, std::vector<std::unique_ptr<Estate>>, TitleComparer> estates;
	std::unordered_set<int> allocatedTerritoryIDs;
	std::unordered_set<std::string> allocatedEstateNames;

	Estate *getFief(std::string name);

	void loadBarony(std::ifstream &file, std::vector<std::unique_ptr<LandTerritory>>& landTerritories);
	void loadMaridom(std::ifstream &file, std::vector<std::unique_ptr<NavalTerritory>>& navalTerritories);
	void loadEstate(std::ifstream &file);
	std::string loadName(std::ifstream &file);
	sf::Color loadColor(std::ifstream &file);
	std::vector<std::string> loadSubfiefNames(std::ifstream &file);
};