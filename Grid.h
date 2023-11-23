#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <string>

const float GRID_SQUARE_SIZE = 30.0f;  // The width/height of one square of a grid on the map.
const std::string gridSaveLabel = "# grid positions";
const std::string defaultColorLabel = "# default color";

// Hash function for sf::Vector2i
struct Vector2iHash 
{
	size_t operator()(const sf::Vector2i& vector) const {
		// Use the std::hash function to hash the components of sf::Vector2i
		std::hash<int> hasher;
		size_t seed = 0;
		seed ^= hasher(vector.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(vector.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

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
	Grid(sf::Color color);
	Grid(sf::Color color, std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions);

	virtual void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;

	// select()  // grid becomes selected and displays border in different colour.

	void setBorderMode(BorderMode borderMode);
	void addGrid(const Grid &grid);  // Adds parameter grid positions to this grids positions.
	void removeGrid(const Grid &grid);  // Removes parameter grid positions from this grids positions.
	void addSquare(sf::Vector2f position);  // Adds grid square at position in world.
	void removeSquare(sf::Vector2f position);  // Adds grid square at position in world.

	bool sharesBorder(const Grid &grid) const;
	bool isEmpty() const;  // Empty is true if there are no grid positions.
	bool containsPosition(sf::Vector2f position) const;  // Returns true if any grid square contains position.
	sf::Vector2f getCenter() const;  // Returns center position in game world of territory.
	void setColor(sf::Color color);

	void calculateCenter();  
	// Borders are added to subBorders. 
	void addBordersToSubBorders();  // Necessary for calculating adjacencies efficiently.

private:


	bool isBorder(sf::Vector2i position) const;  // Returns true if grid position on grid border.
	void calculateVertices();  // Calculates vertices for vertex array from square positions.
	sf::Vector2i calculateGridCoordinates(const sf::Vector2f &position) const;  // Converts vector world position to grid position.
	sf::Vector2f calculateWorldCoordinates(const sf::Vector2i &position) const;  // Converts vector world position to grid position.

	std::unordered_set<sf::Vector2i, Vector2iHash> positions;  // Positions on map grid occupys. 
	std::unordered_set<sf::Vector2i, Vector2iHash> borderAndSubBorderPositions;  // Positions that form sub border or border.
	sf::Vector2i center;  // Center position in game world.
	sf::VertexArray vertices;
	sf::Color color;
	BorderMode borderMode = BorderMode::darkBorders;
};

Grid loadTerritoryGrid(std::ifstream &file);


