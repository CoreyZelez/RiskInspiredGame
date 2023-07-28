#pragma once
#include <SFML/Graphics.hpp>

class TerritoryGraphics
{
public:
	TerritoryGraphics();
	void draw(sf::RenderWindow &window) const;
	void addGridPosition(sf::Vector2f position);
	void setColor(sf::Color color);
	void setDefaultColor(sf::Color color);

private:
	void calculateVertices();  // Calculates vertices for vertex array from square positions.

	const double squareSize = 30;  // The size of one square of a territory on the map.
	std::vector<sf::Vector2f> gridPositions;  // Grid positions on map territory occupys. 
	sf::VertexArray vertices;
	const sf::Color defaultColor;
	sf::Color color = defaultColor;
};

