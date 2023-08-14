#include "Grid.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>

Grid::Grid()
	: color(sf::Color::White)
{
}

Grid::Grid(sf::Color color)
	: color(color)
{
}

Grid::Grid(sf::Color color,
	std::unordered_set<sf::Vector2i, Vector2iHash> positions)
	: color(color), positions(positions)
{
	calculateVertices();
	calculateCenter();
}

void Grid::saveToFile(std::ofstream &file) const
{
	// Write territory grid square locations.
	file << gridSaveLabel << std::endl;
	for(auto iter = positions.begin(); iter != positions.end(); ++iter)
	{
		file << iter->x << " " << iter->y << std::endl;
	}

	// Write color.
	file << defaultColorLabel << std::endl;
	file << static_cast<int>(color.r) << " " 
		<< static_cast<int>(color.g) << " " 
			<< static_cast<int>(color.b) << std::endl;
}

void Grid::draw(sf::RenderWindow &window) const
{
	window.draw(vertices);
}

bool Grid::sharesBorder(const Grid &graphics) const
{
	return false;
}

bool Grid::isEmpty() const
{
	return positions.size() == 0;
}

bool Grid::containsPosition(sf::Vector2f position) const
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	return positions.find(gridPosition) != positions.end();
}

sf::Vector2f Grid::getCenter() const
{
	return center;
}

void Grid::addGrid(const Grid &grid)
{
	for(auto iter = grid.positions.cbegin(); iter != grid.positions.cend(); ++iter)
	{
		positions.insert(*iter);
	}
	calculateVertices();
	calculateCenter();
}

void Grid::removeGrid(const Grid &grid)
{
	for(auto position : grid.positions)
	{
		positions.erase(position);
	}
	calculateVertices();
	calculateCenter();
}

void Grid::addSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Add grid position if not contained in set.
	if(positions.count(gridPosition) == 0)
	{
		positions.emplace(gridPosition);
		calculateVertices();
		calculateCenter();
	}
}

void Grid::removeSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Erase position and recalculate vertices and center if successful.
	if (positions.erase(gridPosition) == 1)
	{
		calculateVertices();
		calculateCenter();
	}
}

void Grid::setColor(sf::Color color)
{
	this->color = color;
	calculateVertices();
}

sf::Vector2i Grid::calculateGridCoordinates(const sf::Vector2f &position) const
{
	const int x = std::floor(position.x / squareSize);
	const int y = std::floor(position.y / squareSize);
	return sf::Vector2i(x, y);
}

bool Grid::isBorder(sf::Vector2i position) const
{
	assert(positions.count(position) == 1);

	sf::Vector2i l = position + sf::Vector2i(-1, 0);
	sf::Vector2i r = position + sf::Vector2i(1, 0);
	sf::Vector2i u = position + sf::Vector2i(0, -1);
	sf::Vector2i d = position + sf::Vector2i(0, 1);
	sf::Vector2i lu = position + sf::Vector2i(-1, -1);
	sf::Vector2i ld = position + sf::Vector2i(-1, 1);
	sf::Vector2i ru = position + sf::Vector2i(1, -1);
	sf::Vector2i rd = position + sf::Vector2i(1, 1);
	std::vector<sf::Vector2i> adjacentPositions = { l, r, u, d, lu, ld, ru, rd };

	for(const sf::Vector2i &position : adjacentPositions)
	{
		if(positions.count(position) == 0)
		{
			return true;
		}
	}
	return false;
}

void Grid::calculateCenter()
{
	sf::Vector2f centerSum;  // Sum of the centers of grid squares.
	for (sf::Vector2i position : positions)
	{
		const float xCenter = (squareSize * position.x) + (squareSize / 2);
		const float yCenter = (squareSize * position.y) + (squareSize / 2);
		centerSum += sf::Vector2f(xCenter, yCenter);
	}
	center = centerSum;
	center.x /= positions.size();
	center.y /= positions.size();
}

void Grid::calculateVertices()
{
	vertices.setPrimitiveType(sf::Triangles);
	const int numTriangles = positions.size() * 6;
	vertices.resize(numTriangles);

	int i = 0;
	for(auto iter = positions.begin(); iter != positions.end(); ++iter)
	{
		const sf::Vector2i position(iter->x, iter->y);
	
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
		if(isBorder(position))
		{
			const sf::Color borderColor(color.r * 0.8, color.g * 0.8, color.b * 0.8);
			triangles[0].color = borderColor;
			triangles[1].color = borderColor;
			triangles[2].color = borderColor;
			triangles[3].color = borderColor;
			triangles[4].color = borderColor;
			triangles[5].color = borderColor;
			++i;
		}
		else
		{
			triangles[0].color = color;
			triangles[1].color = color;
			triangles[2].color = color;
			triangles[3].color = color;
			triangles[4].color = color;
			triangles[5].color = color;
			++i;
		}
	}
}

Grid loadTerritoryGrid(std::ifstream &file)
{
	std::unordered_set<sf::Vector2i, Vector2iHash> positions;

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
			positions.insert(sf::Vector2i(num1, num2));
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

	return Grid(defaultColor, positions);
}