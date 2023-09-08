#include "Grid.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

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

bool Grid::sharesBorder(const Grid &grid) const
{
	for(const sf::Vector2i &position : borderAndSubBorderPositions)
	{
		sf::Vector2i l = sf::Vector2i(position.x - 1, position.y);
		sf::Vector2i r = sf::Vector2i(position.x + 1, position.y);
		sf::Vector2i u = sf::Vector2i(position.x, position.y - 1);
		sf::Vector2i d = sf::Vector2i(position.x, position.y + 1);
		sf::Vector2i lu = sf::Vector2i(position.x - 1, position.y - 1);
		sf::Vector2i ld = sf::Vector2i(position.x - 1, position.y + 1);
		sf::Vector2i ru = sf::Vector2i(position.x + 1, position.y - 1);
		sf::Vector2i rd = sf::Vector2i(position.x + 1, position.y + 1);

		std::vector<sf::Vector2i> adjacentPositions = { l, r, u, d, lu, ld, ru, rd };

		for(const sf::Vector2i &adjacentPosition : adjacentPositions)
		{
			if(grid.borderAndSubBorderPositions.count(adjacentPosition) == 1)
			{
				return true;
			}
		}
	}
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
	return calculateWorldCoordinates(center);
}

void Grid::addGrid(const Grid &grid)
{
	for(auto iter = grid.positions.cbegin(); iter != grid.positions.cend(); ++iter)
	{
		positions.insert(*iter);

		if(grid.borderAndSubBorderPositions.count(*iter) > 0 || grid.isBorder(*iter))
		{
			borderAndSubBorderPositions.insert(*iter);
		}
	}

	calculateVertices();
}

void Grid::removeGrid(const Grid &grid)
{
	for(auto position : grid.positions)
	{
		positions.erase(position);
	}
	calculateVertices();
}

void Grid::addSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Add grid position if not contained in set.
	if(positions.count(gridPosition) == 0)
	{
		positions.emplace(gridPosition);
		calculateVertices();
	}
}

void Grid::removeSquare(sf::Vector2f position)
{
	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Erase position and recalculate vertices and center if successful.
	if (positions.erase(gridPosition) == 1)
	{
		calculateVertices();  
	}
}

void Grid::setColor(sf::Color color)
{
	this->color = color;
	calculateVertices();
}

sf::Vector2i Grid::calculateGridCoordinates(const sf::Vector2f &position) const
{
	const int x = std::floor(position.x / GRID_SQUARE_SIZE);
	const int y = std::floor(position.y / GRID_SQUARE_SIZE);
	return sf::Vector2i(x, y);
}

/* Calculates world coordinate of center of grid square at position. */
sf::Vector2f Grid::calculateWorldCoordinates(const sf::Vector2i &position) const
{
	const float x = position.x * GRID_SQUARE_SIZE + (GRID_SQUARE_SIZE / 2);
	const float y = position.y * GRID_SQUARE_SIZE + (GRID_SQUARE_SIZE / 2);
	return sf::Vector2f(x, y);
}

void Grid::addBordersToSubBorders()
{
	for(auto position : positions)
	{
		if(isBorder(position))
		{
			borderAndSubBorderPositions.insert(position);
		}
	}
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
	if(positions.size() == 0)
	{
		return;
	}

	// Manhattan distances of positions from border.
	std::unordered_map<sf::Vector2i, int, Vector2iHash> distances;

	// Determines which positions lie on grid edge/border.
	for(auto position = positions.cbegin(); position != positions.cend(); ++position)
	{
		if(isBorder(*position))
		{
			distances[*position] = 0;
		}
	}

	// Determines all other distances progressively i.e. distances 1 then 2 then 3 etc.
	bool distancesAltered = true;
	while(distancesAltered)
	{
		// Use a stored copy of distances before iteration since otherwise distance calculation won't be progressive.
		std::unordered_map<sf::Vector2i, int, Vector2iHash> prevDistances = distances;
		distancesAltered = false;
		// Determines distance of a given position only when an adjacent position has had its distance determined.
		for(auto position = positions.cbegin(); position != positions.cend(); ++position)
		{
			// Distance is already determined.
			if(prevDistances.count(*position) == 1)
			{
				continue;
			}

			sf::Vector2i left(position->x - 1, position->y);
			sf::Vector2i right(position->x + 1, position->y);
			sf::Vector2i up(position->x, position->y - 1);
			sf::Vector2i down(position->x, position->y + 1);
			std::vector<sf::Vector2i> adjacencies = { left, right, up, down };
			// Determine minimum distance value of adjacent points.
			int min = INT_MAX;  // min distance of adjacent positions.
			for(const sf::Vector2i &adjacency : adjacencies)
			{
				if(prevDistances.count(adjacency) == 1)
				{
					assert(prevDistances[adjacency] < INT_MAX);
					min = std::min(min, prevDistances[adjacency]);
				}
			}
			if(min != INT_MAX)
			{
				const int distance = min + 1;
				distances[*position] = distance;
				distancesAltered = true;
			}
		}
	}

	// Collects positions with greatest manhattan distance from edge.
	std::vector<sf::Vector2i> candidatePositions; 
	int maxDist = 0;
	const int distLenience = 1;
	for(auto position = positions.cbegin(); position != positions.cend(); ++position)
	{
		assert(distances.count(*position) == 1);
		if(distances[*position] > maxDist)
		{
			maxDist = distances[*position];
			candidatePositions.clear();
			candidatePositions.push_back(*position);
		}
		else if(distances[*position] == maxDist)
		{
			candidatePositions.push_back(*position);
		}
	}

	assert(candidatePositions.size() > 0);
	// Determine average position of candidate position.
	float xSum = 0;
	float ySum = 0;
	for(const sf::Vector2i& position : candidatePositions)
	{
		xSum += position.x;
		ySum += position.y;
	}
	const float xAvg = xSum / candidatePositions.size();
	const float yAvg = ySum / candidatePositions.size();
	sf::Vector2f candidateAverage(xAvg, yAvg);  


	// Determines candidate position closest to candidate average position. This will be the center.
	float minDist = FLT_MAX;
	for(const sf::Vector2i& position : candidatePositions)
	{
		const float dist = abs(position.x - candidateAverage.x) + abs(position.y - candidateAverage.y);
		if(dist < minDist)
		{
			center = position;
			minDist = dist;
		}
	}
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
	
		const float left = position.x * GRID_SQUARE_SIZE;
		const float right = (position.x * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;
		const float top = (position.y * GRID_SQUARE_SIZE);
		const float bottom = (position.y * GRID_SQUARE_SIZE) + GRID_SQUARE_SIZE;
	
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
		if((borderMode == BorderMode::feintBorders || borderMode == BorderMode::darkBorders) 
			&& isBorder(position))
		{
			if(borderMode == BorderMode::feintBorders)
			{
				const sf::Color borderColor(color.r * 0.8, color.g * 0.8, color.b * 0.8);
				triangles[0].color = borderColor;
				triangles[1].color = borderColor;
				triangles[2].color = borderColor;
				triangles[3].color = borderColor;
				triangles[4].color = borderColor;
				triangles[5].color = borderColor;
			}
			else if(borderMode == BorderMode::darkBorders)
			{
				const sf::Color borderColor(0, 0, 0);
				triangles[0].color = borderColor;
				triangles[1].color = borderColor;
				triangles[2].color = borderColor;
				triangles[3].color = borderColor;
				triangles[4].color = borderColor;
				triangles[5].color = borderColor;
			}
		}
		else if(borderMode == BorderMode::darkBorders && borderAndSubBorderPositions.count(position) > 0)
		{
			const sf::Color borderColor(color.r * 0.4, color.g * 0.4, color.b * 0.4);
			triangles[0].color = borderColor;
			triangles[1].color = borderColor;
			triangles[2].color = borderColor;
			triangles[3].color = borderColor;
			triangles[4].color = borderColor;
			triangles[5].color = borderColor;
		}
		else
		{
			triangles[0].color = color;
			triangles[1].color = color;
			triangles[2].color = color;
			triangles[3].color = color;
			triangles[4].color = color;
			triangles[5].color = color;
		}

		++i;
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

	Grid grid(defaultColor, positions);
	grid.calculateCenter();

	// Necessary for efficient computation of adjacencies.
	grid.addBordersToSubBorders();

	return grid;
}
