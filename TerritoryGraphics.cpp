#include "TerritoryGraphics.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <assert.h>

TerritoryGraphics::TerritoryGraphics(sf::Color defaultColor)
	: defaultColor(defaultColor)
{
	
}

void TerritoryGraphics::draw(sf::RenderWindow & window) const
{
	window.draw(vertices);
}

bool TerritoryGraphics::sharesBorder(const TerritoryGraphics & graphics) const
{
	return false;
}

bool TerritoryGraphics::isEmpty() const
{
	return gridPositions.size() == 0;
}

bool TerritoryGraphics::containsPosition(sf::Vector2f position) const
{
	// Converts position to grid coordinates.
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;
	sf::Vector2i gridPosition(x, y);
	return gridPositions.find(gridPosition) != gridPositions.end();
}

sf::Vector2f TerritoryGraphics::getCenter() const
{
	return center;
}

void TerritoryGraphics::addSquare(sf::Vector2f position)
{
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;
	sf::Vector2i gridPosition(x, y);
	// Add grid position if not contained in set.
	if(gridPositions.count(gridPosition) == 0)
	{
		gridPositions.emplace(gridPosition);
		calculateVertices();
		calculateCenter();
	}
}

void TerritoryGraphics::removeSquare(sf::Vector2f position)
{
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;
	// Erase position and recalculate vertices and center if successful.
	if (gridPositions.erase(sf::Vector2i(x, y)) == 1)
	{
		calculateVertices();
		calculateCenter();
	}
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

void TerritoryGraphics::calculateCenter()
{
	sf::Vector2f centerSum;  // Sum of the centers of grid squares.
	for (sf::Vector2i position : gridPositions)
	{
		const float xCenter = (squareSize * position.x) - (squareSize / 2);
		const float yCenter = (squareSize * position.y) + (squareSize / 2);
		centerSum += sf::Vector2f(xCenter, yCenter);
	}
	center = centerSum;
	center.x /= gridPositions.size();
	center.y /= gridPositions.size();
}

void TerritoryGraphics::calculateVertices()
{
	vertices.setPrimitiveType(sf::Triangles);
	const int numTriangles = gridPositions.size() * 6;
	vertices.resize(numTriangles);

	int i = 0;
	for(auto iter = gridPositions.begin(); iter != gridPositions.end(); ++iter)
	{
		const sf::Vector2f position(iter->x, iter->y);
	
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
		++i;
	}
}
