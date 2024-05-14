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

class EditorGrid  
{
public:
	EditorGrid();
	explicit EditorGrid(sf::Color color);
	EditorGrid(sf::Color color, std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions);
	// Constructor for testing purposes. 
	explicit EditorGrid(std::unordered_set<sf::Vector2i, Vector2iHash> positions);

	void update();

	virtual void saveToFile(std::ofstream &file) const;

	//// MAYBE WORK TO REMOVE THIS. SHOULD MAYBE COMBINE ALL VERTEX ARRAYS FROM EACH GRID INTO SINGLE VERTEX ARRAY AND DRAW THAT.
	void draw(sf::RenderWindow &window) const;

	const sf::VertexArray& getVertexArray() const;

	void setBorderMode(BorderMode borderMode);

	// Specifies every position in grid as a dark position.
	void setAllPositionsDark();

	// Adds parameter grid positions to this grids positions.
	void addGrid(const EditorGrid &grid, bool updateVertices = true);

	// Removes parameter grid positions from this grids positions.
	void removeGrid(const EditorGrid &grid, bool updateVertices = true);

	// Adds grid square at position in world. Should only be used by mapmaker.
	// Additionally requires subBorderPositions is empty
	void addPosition(sf::Vector2i gridPosition, bool updateVertices = true);

	// Removes grid square at position in world. Should only be used by mapmaker.
	// Additionally requires subBorderPositions is empty
	void removePosition(sf::Vector2i gridPosition, bool updateVertices = true);

	// Removes grid square at position in world. Returns true if a square is removed.
	bool removeSquare(sf::Vector2f position, bool updateVertices = true);

	bool sharesBorder(const EditorGrid &grid) const;

	// Returns world coordinates of positions that lie on border with specified grid.
	std::unordered_set<sf::Vector2f, Vector2fHash> getNeighbouringBorderPositions(const EditorGrid &grid) const;

	// Returns grid positions.
	std::unordered_set<sf::Vector2i, Vector2iHash> getPositions() const;
	
	// Empty is true if there are no grid positions.
	bool isEmpty() const;  

	bool isOutdated() const;

	// Returns true if any grid contains world position.
	bool containsPosition(sf::Vector2f position) const;  
	// Returns true if any grid contains frid position.
	bool containsPosition(sf::Vector2i position) const;  

	sf::Vector2f getCenter() const;  // Returns center position in game world of territory.

	void setColor(sf::Color color);
	const sf::Color& getColor() const; 

	void calculateCenter();  

private:
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;  // Positions on map grid occupys. 
	std::unordered_set<sf::Vector2i, Vector2iHash> darkPositions;  // Positions of grid which are partially darkened.
	std::unordered_set<sf::Vector2i, Vector2iHash> subBorderPositions;  // Positions that form sub border or border.
	std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions;  // Positions that form sub border or border.
	bool outdated = false;  // Indicated that calculateVertices must be called before next time grid is drawn.
	sf::Vector2i center;  // Center position in game world.
	sf::VertexArray vertices;
	sf::Color color;
	BorderMode borderMode = BorderMode::darkBorders;

	// Determines which positions are borders and adds them to borderPositions.
	void initBorders();
	// Returns true if grid position on grid border.
	bool isBorder(sf::Vector2i position) const;  
	// Calculates vertices for vertex array from square positions.
	void calculateVertices();  
	// Returns positions adjacent to border positions not contained in grid.
	std::unordered_set<sf::Vector2i, Vector2iHash> getAdjacentPositions() const;
	// Returns color adjusted for dark specifier.
	sf::Color calculateAdjustedColor(const sf::Vector2i &position);
};

EditorGrid loadTerritoryGrid(std::ifstream &file);

sf::Vector2i calculateGridCoordinates(const sf::Vector2f &position);
// Converts vector world position to grid position.
sf::Vector2f calculateWorldCoordinates(const sf::Vector2i &position);  

std::vector<sf::Vector2i> calculateAdjacentPositions(const sf::Vector2i &position);


