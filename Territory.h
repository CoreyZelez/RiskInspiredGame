#pragma once
#include "Subject.h"
#include "TerritoryGraphics.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player;
class LandArmy;
class NavalFleet;

const std::string landSaveLabel = "# land";
const std::string navalSaveLabel = "# naval";

class Territory : public Subject
{
public:
	explicit Territory(TerritoryGraphics graphics);
	explicit Territory(sf::Color color);

	void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;  // In future return vertex arrays probably!!!

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) = 0;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) = 0;

	/// JUST ADD GETTER TO TERRITORY GRAPHICS THEN ELIMINATE THESE FUNCTIONS!!!

	// Adds square at position in game world.
	void addSquare(sf::Vector2f position);  
	// Removes square at position in game world.
	void removeSquare(sf::Vector2f position);  

	// Returns true if any grid square of territory contains position.
	bool containsPosition(sf::Vector2f position) const;  
	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;

	double getDefenceMultiplier() const;

	virtual const Player *getOccupant();

	// True if territory occupies no positions on map.
	bool isEmpty() const;  

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const = 0;

	sf::Vector2f getCenter() const;

private:
	TerritoryGraphics graphics;
	double defenceMultiplier = 1;  // In future perhaps have complex virtual function to calculate this!
};


