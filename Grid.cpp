#include "Grid.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <execution>

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
	initBorders();
	calculateVertices();
}


//
///
// WTF IS THIS CONSTRUCTOR?
//
// FIND WHERE IT IS USED IF AT ALL!!!
Grid::Grid(std::unordered_set<sf::Vector2i, Vector2iHash> borderPositions)
	: color(sf::Color::White), positions(borderPositions), borderPositions(borderPositions)
{
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

const sf::VertexArray& Grid::getVertexArray() const
{
	return vertices;
}

bool Grid::sharesBorder(const Grid &grid) const
{
	for(const sf::Vector2i &position : borderPositions)
	{
		std::vector<sf::Vector2i> adjacentPositions = calculateAdjacentPositions(position);

		for(const sf::Vector2i &adjacentPosition : adjacentPositions)
		{
			if(grid.borderPositions.count(adjacentPosition) == 1)
			{
				return true;
			}
		}
	}
	return false;
}

std::unordered_set<sf::Vector2f, Vector2fHash> Grid::getNeighbouringBorderPositions(const Grid &grid) const
{
	std::unordered_set<sf::Vector2i, Vector2iHash> neighbouringBorderPositions = {};
	for(auto position = borderPositions.cbegin(); position != borderPositions.cend(); ++position)
	{
		std::vector<sf::Vector2i> adjacentPositions = calculateAdjacentPositions(*position);

		for(sf::Vector2i position : adjacentPositions)
		{
			if(grid.borderPositions.count(position) == 1)
			{
				neighbouringBorderPositions.insert(position);
				break;
			}
			else
			{
				// Verifies that position does not exist at all in grid since otherwise it would be a border position.
				assert(grid.positions.count(position) == 0);
			}
		}
	}

	// Convert positions to global coordinates.
	std::unordered_set<sf::Vector2f, Vector2fHash> neighbouringBorderCoordinates = {};
	for(const sf::Vector2i &position : neighbouringBorderPositions)
	{
		neighbouringBorderCoordinates.insert(calculateWorldCoordinates(position));
	}
	return neighbouringBorderCoordinates;
}

std::unordered_set<sf::Vector2i, Vector2iHash> Grid::getPositions() const
{
	return positions;
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

void Grid::setBorderMode(BorderMode borderMode)
{
	this->borderMode = borderMode;
	setColor(color);  // Updates grid with no border mode.
}

void Grid::addGrid(const Grid &grid, bool updateVertices)
{
	// Border positions from parameter grid that may possibly be border positions of this grid once added.
	std::vector<sf::Vector2i> candidateBorderPositions = {};
	// Iterate through positions determining sub-border positions and candidate border positions.
	for(auto iter = grid.positions.cbegin(); iter != grid.positions.cend(); ++iter)
	{
		positions.insert(*iter);
		// Only a border position of parameter grid can become border position of this grid.
		if(grid.borderPositions.count(*iter) == 1)
		{
			candidateBorderPositions.push_back(*iter);
		}
		else if(grid.subBorderPositions.count(*iter) == 1)
		{
			subBorderPositions.insert(*iter);
		}
	}

	// Determing border and sub-border positions amongst candidate border positions.
	for(const auto &position : candidateBorderPositions)
	{
		if(isBorder(position))
		{
			borderPositions.insert(position);
		}
		else
		{
			subBorderPositions.insert(position);
		}
	}

	// Transfer border positions that become sub-border positions due to added grid.
	const std::vector<sf::Vector2i> potentialBorderToSubBorderPositions = grid.getAdjacentPositions();
	for(const sf::Vector2i &candidatePosition : potentialBorderToSubBorderPositions)
	{
		// Determine where the candidate position is currently classified as a border position.
		if(borderPositions.count(candidatePosition) == 1)
		{
			// Transfer the position if it is no longer a border position.
			if(!isBorder(candidatePosition))
			{
				borderPositions.erase(candidatePosition);
				subBorderPositions.insert(candidatePosition);
			}
		}
	}

	// JUST AN IDEA WHAT TO DO FOR DIFFERENTIATING GAME FROM MAP MAKER HANDLING OF GRIDS
	if(updateVertices)
	{
		calculateVertices();
	}
}

void Grid::removeGrid(const Grid &grid, bool updateVertices)
{
	for(const sf::Vector2i &position : grid.positions)
	{
		positions.erase(position);
		subBorderPositions.erase(position);
		borderPositions.erase(position);
	}

	// Transfer sub-border positions that become border positions due to removed grid.
	const std::vector<sf::Vector2i> potentialSubBorderToBorderPositions = grid.getAdjacentPositions();
	for(const sf::Vector2i &candidatePosition : potentialSubBorderToBorderPositions)
	{
		// Determine where the candidate position is currently classified as a border position.
		if(subBorderPositions.count(candidatePosition) == 1)
		{
			// Transfer the position if it is no longer a border position.
			if(isBorder(candidatePosition))
			{
				subBorderPositions.erase(candidatePosition);
				borderPositions.insert(candidatePosition);
			}
		}
	}

	// JUST AN IDEA WHAT TO DO FOR DIFFERENTIATING GAME FROM MAP MAKER HANDLING OF GRIDS
	if(updateVertices)
	{
		calculateVertices();
	}

}

void Grid::addPosition(sf::Vector2i gridPosition, bool updateVertices)
{
	assert(subBorderPositions.size() == 0);
	// Add grid position if not contained in set.
	if(positions.count(gridPosition) == 0)
	{
		positions.insert(gridPosition);
		if(isBorder(gridPosition))
		{
			borderPositions.insert(gridPosition);
		}

		// Convert border positions adjacent to position to non-border positions if necessary.
		std::vector<sf::Vector2i> adjacencies = calculateAdjacentPositions(gridPosition);
		for(const sf::Vector2i &adjacency : adjacencies)
		{
			if(borderPositions.count(adjacency) == 1 && !isBorder(adjacency))
			{
				borderPositions.erase(adjacency);
			}
		}

		if(updateVertices)
		{
			calculateVertices();
		}
	}
}


bool Grid::removeSquare(sf::Vector2f position, bool updateVertices)
{
	assert(subBorderPositions.size() == 0);

	sf::Vector2i gridPosition = calculateGridCoordinates(position);
	// Erase position and recalculate vertices and center if successful.
	if (positions.erase(gridPosition) == 1)
	{
		borderPositions.erase(gridPosition);

		// Convert ordinary positions adjacent to position to border positions if necessary.
		std::vector<sf::Vector2i> adjacencies = calculateAdjacentPositions(gridPosition);
		for(const sf::Vector2i &adjacency : adjacencies)
		{
			if(positions.count(adjacency) == 1 && borderPositions.count(adjacency) == 0 && isBorder(adjacency))
			{
				borderPositions.insert(adjacency);
			}
		}

		if(updateVertices)
		{
			calculateVertices();
		}

		return true;
	}

	return false;
}

void Grid::setColor(sf::Color color)
{
	this->color = color;
	calculateVertices();
}

void Grid::initBorders()
{
	assert(borders.size() == 0);
	for(auto position : positions)
	{
		if(isBorder(position))
		{
			borderPositions.insert(position);
		}
	}
}

bool Grid::isBorder(sf::Vector2i position) const
{
	assert(positions.count(position) == 1);

	std::vector<sf::Vector2i> adjacentPositions = calculateAdjacentPositions(position);

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
		// We check that it is in borderAndSubBorderPositions first as it is cheaper than checking isBorder.
		if((borderMode == BorderMode::feintBorders || borderMode == BorderMode::darkBorders) 
			&& borderPositions.count(position) == 1)
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
		else if(borderMode == BorderMode::darkBorders && subBorderPositions.count(position) == 1)
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

std::vector<sf::Vector2i> Grid::getAdjacentPositions() const
{
	// Adjacent positions to this grid.
	std::vector<sf::Vector2i> adjacencies;
	for(const sf::Vector2i &position : positions)
	{
		sf::Vector2i l = sf::Vector2i(position.x - 1, position.y);
		sf::Vector2i r = sf::Vector2i(position.x + 1, position.y);
		sf::Vector2i u = sf::Vector2i(position.x, position.y - 1);
		sf::Vector2i d = sf::Vector2i(position.x, position.y + 1);
		sf::Vector2i lu = sf::Vector2i(position.x - 1, position.y - 1);
		sf::Vector2i ld = sf::Vector2i(position.x - 1, position.y + 1);
		sf::Vector2i ru = sf::Vector2i(position.x + 1, position.y - 1);
		sf::Vector2i rd = sf::Vector2i(position.x + 1, position.y + 1);

		std::vector<sf::Vector2i> candidateAdjacencies = { l, r, u, d, lu, ld, ru, rd };
		for(sf::Vector2i &candidateAdjacency : candidateAdjacencies)
		{
			if(positions.count(candidateAdjacency) == 0)
			{
				adjacencies.push_back(candidateAdjacency);
			}
		}
	}
	return adjacencies;
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
	grid.initBorders();

	return grid;
}

sf::Vector2i calculateGridCoordinates(const sf::Vector2f & position)
{
	const int x = std::floor(position.x / GRID_SQUARE_SIZE);
	const int y = std::floor(position.y / GRID_SQUARE_SIZE);
	return sf::Vector2i(x, y);
}

sf::Vector2f calculateWorldCoordinates(const sf::Vector2i & position)
{
	const float x = position.x * GRID_SQUARE_SIZE + (GRID_SQUARE_SIZE / 2);
	const float y = position.y * GRID_SQUARE_SIZE + (GRID_SQUARE_SIZE / 2);
	return sf::Vector2f(x, y);
}

std::vector<sf::Vector2i> calculateAdjacentPositions(const sf::Vector2i &position)
{
	sf::Vector2i l = sf::Vector2i(position.x - 1, position.y);
	sf::Vector2i r = sf::Vector2i(position.x + 1, position.y);
	sf::Vector2i u = sf::Vector2i(position.x, position.y - 1);
	sf::Vector2i d = sf::Vector2i(position.x, position.y + 1);
	sf::Vector2i lu = sf::Vector2i(position.x - 1, position.y - 1);
	sf::Vector2i ld = sf::Vector2i(position.x - 1, position.y + 1);
	sf::Vector2i ru = sf::Vector2i(position.x + 1, position.y - 1);
	sf::Vector2i rd = sf::Vector2i(position.x + 1, position.y + 1);

	return { l, r, u, d, lu, ld, ru, rd };
}
