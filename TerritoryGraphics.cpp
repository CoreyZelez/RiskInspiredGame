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

bool TerritoryGraphics::isEmpty() const
{
	return gridPositions.size() == 0;
}

bool TerritoryGraphics::containsPosition(sf::Vector2f position) const
{
	// Converts position to grid coordinates.
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;

	auto iter = gridPositions.begin();
	while(iter != gridPositions.end())
	{
		if(iter->x == x && iter->y == y)
		{
			return true;
		}
		++iter;
	}

	return false;
}

void TerritoryGraphics::addSquare(sf::Vector2f position)
{
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;
	sf::Vector2i gridPos(x, y);
	if(std::find(gridPositions.begin(), gridPositions.end(), gridPos) == gridPositions.end())
	{
		gridPositions.push_back(sf::Vector2i(x, y));
		calculateVertices();
	}
}

void TerritoryGraphics::removeSquare(sf::Vector2f position)
{
	const int x = position.x / squareSize;
	const int y = position.y / squareSize;
	auto iter = gridPositions.begin();
	while(iter != gridPositions.end())
	{
		if(iter->x == x && iter->y == y)
		{
			gridPositions.erase(iter);
			calculateVertices();
			return;
		}
		++iter;
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

void TerritoryGraphics::calculateVertices()
{
	vertices.setPrimitiveType(sf::Triangles);
	const int numTriangles = gridPositions.size() * 6;
	vertices.resize(numTriangles);

	// Populate the vertex array, with two triangles per square tile.
	for(int i = 0; i < gridPositions.size(); ++i)
	{
		const sf::Vector2f position(gridPositions[i].x, gridPositions[i].y);

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
