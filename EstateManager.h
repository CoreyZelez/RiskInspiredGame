#pragma once
#include "Title.h"
#include "Estate.h"
#include "LandedEstate.h"
#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class LandTerritory;
class CoastalTerritory;

class EstateManager
{
public:
	void save(std::string mapName) const;
	void load(std::string mapName);

	void generateBaronies(std::vector<std::unique_ptr<LandTerritory>> &landTerritories);

	Estate* createEstate(Title title);  // Creates estate with title and returns handle.
	void removeEstate(Estate **estate);  // Removes estate and nulls pointer handle.
	Estate* getEstate(sf::Vector2f position);  // Returns pointer to land territory at world position.

private:
	std::vector<std::unique_ptr<Estate>> estates;
	std::vector<std::unique_ptr<LandedEstate>> landedEstates;
};

