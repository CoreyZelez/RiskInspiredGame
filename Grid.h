#pragma once
#include "HashFunctions.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <string>

const float GRID_SQUARE_SIZE = 30.0f;  // The width/height of one square of a grid on the map.
const std::string gridSaveLabel = "# grid positions";
const std::string defaultColorLabel = "# default color";

enum class BorderMode
{
	noBorders,
	feintBorders,
	darkBorders
};

class Grid  
{
public:
	Grid();
	explicit Grid(sf::Color color);
	Grid(sf::Color color, std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions);
	// Constructor for testing purposes. Ensures grid contains border positions.
	explicit Grid(std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions);

	virtual void saveToFile(std::ofstream &file) const;

	// WORK TO REMOVE THIS. SHOULD COMBINE ALL VERTEX ARRAYS FROM EACH GRID INTO SINGLE VERTEX ARRAY AND DRAW THAT.
	void draw(sf::RenderWindow &window) const;

	const sf::VertexArray& getVertexArray() const;

	// select()  // grid becomes selected and displays border in different colour.

	void setBorderMode(BorderMode borderMode);

	// Adds parameter grid positions to this grids positions.
	void addGrid(const Grid &grid);  

	// Removes parameter grid positions from this grids positions.
	void removeGrid(const Grid &grid);  

	// Adds grid square at position in world.
	void addSquare(sf::Vector2i gridPosition);

	// Removes grid square at position in world. Returns true if a square is removed.
	bool removeSquare(sf::Vector2f position);  

	bool sharesBorder(const Grid &grid) const;

	// Returns world coordinates of positions that lie on border with specified grid.
	std::vector<sf::Vector2f> getNeighbouringBorderPositions(const Grid &grid) const;

	// Returns grid positions.
	std::unordered_set<sf::Vector2i, Vector2iHash> getPositions() const;

	bool isEmpty() const;  // Empty is true if there are no grid positions.

	bool containsPosition(sf::Vector2f position) const;  // Returns true if any grid square contains position.

	sf::Vector2f getCenter() const;  // Returns center position in game world of territory.

	void setColor(sf::Color color);

	void calculateCenter();  
	// Borders are added to subBorders. 
	void addBordersToSubBorders();  // Necessary for calculating adjacencies efficiently.

private:
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;  // Positions on map grid occupys. 
	std::unordered_set<sf::Vector2i, Vector2iHash> borderAndSubBorderPositions;  // Positions that form sub border or border.
	sf::Vector2i center;  // Center position in game world.
	sf::VertexArray vertices;
	sf::Color color;
	BorderMode borderMode = BorderMode::darkBorders;

	bool isBorder(sf::Vector2i position) const;  // Returns true if grid position on grid border.
	void calculateVertices();  // Calculates vertices for vertex array from square positions.
};

Grid loadTerritoryGrid(std::ifstream &file);

sf::Vector2i calculateGridCoordinates(const sf::Vector2f &position);
// Converts vector world position to grid position.
sf::Vector2f calculateWorldCoordinates(const sf::Vector2i &position);  


