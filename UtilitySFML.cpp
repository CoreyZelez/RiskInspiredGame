#include <SFML/Graphics.hpp>
#include <vector>

bool isPointInsideTriangle(const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
{
	float denominator = ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denominator;
	float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denominator;
	float gamma = 1.0f - alpha - beta;

	return alpha > 0 && beta > 0 && gamma > 0;
}

bool isEar(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const std::vector<sf::Vector2f>& polygon)
{
	for(size_t i = 0; i < polygon.size(); ++i)
	{
		const sf::Vector2f& p = polygon[i];
		const sf::Vector2f& q = polygon[(i + 1) % polygon.size()];
		const sf::Vector2f& r = polygon[(i + 2) % polygon.size()];

		if(p != a && p != b && p != c && isPointInsideTriangle(p, a, b, c))
		{
			return false;
		}
	}

	return true;
}

sf::VertexArray triangulatePolygon(const std::vector<sf::Vector2f>& polygon)
{
	sf::VertexArray triangles(sf::Triangles);

	std::vector<sf::Vector2f> remainingVertices = polygon;

	while(remainingVertices.size() >= 3)
	{
		for(size_t i = 0; i < remainingVertices.size(); ++i)
		{
			const sf::Vector2f& a = remainingVertices[i];
			const sf::Vector2f& b = remainingVertices[(i + 1) % remainingVertices.size()];
			const sf::Vector2f& c = remainingVertices[(i + 2) % remainingVertices.size()];

			if(isEar(a, b, c, remainingVertices))
			{
				triangles.append(sf::Vertex(a));
				triangles.append(sf::Vertex(b));
				triangles.append(sf::Vertex(c));

				remainingVertices.erase(remainingVertices.begin() + (i + 1) % remainingVertices.size());
				break;
			}
		}
	}

	return triangles;
}