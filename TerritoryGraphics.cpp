#include "TerritoryGraphics.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>

TerritoryGraphics::TerritoryGraphics(sf::Color defaultColor)
	: defaultColor(defaultColor)
{
	
}

TerritoryGraphics::TerritoryGraphics(sf::Color defaultColor, 
	std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions)
	: defaultColor(defaultColor), gridPositions(gridPositions)
{
	calculateVertices();
	calculateCenter();
}

void TerritoryGraphics::saveToFile(std::ofstream &file) const
{
	// Write territory grid square locations.
	file << gridSaveLabel << std::endl;
	for(auto iter = gridPositions.begin(); iter != gridPositions.end(); ++iter)
	{
		file << iter->x << " " << iter->y << std::endl;
	}

	// Write territory default color.
	file << defaultColorLabel << std::endl;
	file << static_cast<int>(defaultColor.r) << " " 
		<< static_cast<int>(defaultColor.g) << " " 
			<< static_cast<int>(defaultColor.b) << std::endl;
}

void TerritoryGraphics::draw(sf::RenderWindow &window) const
{
	window.draw(vertices);
}

bool TerritoryGraphics::sharesBorder(const TerritoryGraphics &graphics) const
{
	return false;
}

bool TerritoryGraphics::isEmpty() const
{
	return gridPositions.size() == 0;
}

bool TerritoryGraphics::containsPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	return gridPositions.find(gridPosition) != gridPositions.end();
}

sf::Vector2f TerritoryGraphics::getCenter() const
{
	return center;
}

void TerritoryGraphics::addSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Add grid position if not contained in set.
	if(gridPositions.count(gridPosition) == 0)
	{
		gridPositions.emplace(gridPosition);
		calculateVertices();
		calculateCenter();
	}
	calculateVertices();
}

void TerritoryGraphics::removeSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Erase position and recalculate vertices and center if successful.
	if (gridPositions.erase(gridPosition) == 1)
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

sf::Vector2i TerritoryGraphics::calculateGridCoordinates(const sf::Vector2f &position) const
{
	const int x = std::floor(position.x / squareSize);
	const int y = std::floor(position.y / squareSize);
	return sf::Vector2i(x, y);
}

void TerritoryGraphics::calculateCenter()
{
	sf::Vector2f centerSum;  // Sum of the centers of grid squares.
	for (sf::Vector2i position : gridPositions)
	{
		const float xCenter = (squareSize * position.x) + (squareSize / 2);
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

TerritoryGraphics loadTerritoryGraphics(std::ifstream &file)
{
	std::unordered_set<sf::Vector2i, Vector2iHash> gridPositions;

	std::string line;
	int num1;
	int num2;

	std::getline(file, line);
	assert(line.compare(gridSaveLabel) == 0);
	while(std::getline(file, line) && line.size() > 0 && line[0] != '#')
	{
		std::istringstream iss(line);
		if(iss >> num1 >> num2)
		{
			gridPositions.insert(sf::Vector2i(num1, num2));
		}
		else
		{
			// Error with file format.
			exit(1);
		}
	}

	// Determine default color from file.
	std::getline(file, line);
	std::istringstream iss(line);
	int r, g, b;
	iss >> r >> g >> b;
	sf::Color defaultColor(r, g, b);

	return TerritoryGraphics(defaultColor, gridPositions);
}
