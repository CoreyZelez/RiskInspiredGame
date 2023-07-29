#pragma once
#include <SFML/Graphics.hpp>

class TerritoryGraphics
{
public:
	TerritoryGraphics();

	void draw(sf::RenderWindow &window) const;

	void addSquare(sf::Vector2f position);  // Adds grid square at position in world.
	void removeSquare(sf::Vector2f position);  // Adds grid square at position in world.

	bool isEmpty() const;  // Empty is true if there are no grid positions.
	bool containsPosition(sf::Vector2f position) const;  // Returns true if any grid square contains position.

	void setColor(sf::Color color);
	void setDefaultColor(sf::Color color);

private:
	void calculateVertices();  // Calculates vertices for vertex array from square positions.

	const double squareSize = 30;  // The size of one square of a territory on the map.
	std::vector<sf::Vector2i> gridPositions;  // Grid positions on map territory occupys. 
	sf::VertexArray vertices;
	const sf::Color defaultColor;
	sf::Color color = defaultColor;
};

