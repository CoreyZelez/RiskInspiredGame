#include "TerritoryGraphics.h"

TerritoryGraphics::TerritoryGraphics()
	: defaultColor(sf::Color::Red)
{
	
}

void TerritoryGraphics::draw(sf::RenderWindow & window) const
{
	window.draw(vertices);
}

void TerritoryGraphics::addGridPosition(sf::Vector2f position)
{
	gridPositions.push_back(position);
	calculateVertices();
}

void TerritoryGraphics::setColor(sf::Color color)
{
	this->color = color;
	calculateVertices();
}

void TerritoryGraphics::setDefaultColor(sf::Color color)
{
	color = defaultColor;
	calculateVertices();
}

void TerritoryGraphics::calculateVertices()
{
	vertices.setPrimitiveType(sf::Triangles);
	const int numTriangles = gridPositions.size() * 6;
	vertices.resize(numTriangles);

	// Populate the vertex array, with two triangles per square tile.
	for(int i = 0; i < gridPositions.size(); ++i)
	{
		const sf::Vector2f position = gridPositions[i];

		const float left = position.x * squareSize;
		const float right = (position.x * squareSize) + squareSize;
		const float top = (position.y * squareSize);
		const float bottom = (position.y * squareSize) + squareSize;

		// Pointer to the triangles' vertices of the current tile.
		sf::Vertex* triangles = &vertices[i * 6];

		// define the 6 corners of the two triangles
		triangles[0].position = sf::Vector2f(left, top);
		triangles[1].position = sf::Vector2f(left, bottom);
		triangles[2].position = sf::Vector2f(right, top);
		triangles[3].position = sf::Vector2f(right, top);
		triangles[4].position = sf::Vector2f(right, bottom);
		triangles[5].position = sf::Vector2f(left, bottom);

		// define the 6 matching texture coordinates
		triangles[0].color = color;
		triangles[1].color = color;
		triangles[2].color = color;
		triangles[3].color = color;
		triangles[4].color = color;
		triangles[5].color = color;
	}
}
