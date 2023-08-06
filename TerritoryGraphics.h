#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <string>

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

class TerritoryGraphics  // In future rename to TerritoryGrid. Do so when you stop having this class draw itself!!!
{
public:
	TerritoryGraphics(sf::Color defaultColor);
	TerritoryGraphics(sf::Color defaultColor, std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions);

	virtual void saveToFile(std::ofstream &file) const;

	void draw(sf::RenderWindow &window) const;

	void addSquare(sf::Vector2f position);  // Adds grid square at position in world.
	void removeSquare(sf::Vector2f position);  // Adds grid square at position in world.

	bool sharesBorder(const TerritoryGraphics &graphics) const;
	bool isEmpty() const;  // Empty is true if there are no grid positions.
	bool containsPosition(sf::Vector2f position) const;  // Returns true if any grid square contains position.

	sf::Vector2f getCenter() const;  // Returns center position in game world of territory.

	void setColor(sf::Color color);
	void setDefaultColor(sf::Color color);

private:
	sf::Vector2i calculateGridCoordinates(const sf::Vector2f &position) const;  // Converts vector world position to grid position.

	void calculateCenter();
	void calculateVertices();  // Calculates vertices for vertex array from square positions.

	const float squareSize = 30.0f;  // The size of one square of a territory on the map.

	std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions;  // Grid positions on map territory occupys. 
	sf::Vector2f center;  // Center position in game world.

	sf::VertexArray vertices;

	const sf::Color defaultColor;
	sf::Color color = defaultColor;
};

TerritoryGraphics loadTerritoryGraphics(std::ifstream &file);


