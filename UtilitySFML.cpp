#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>
#include "UtilitySFML.h"

// Helper function for pointInTriangle
float sign(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// Function to check if a point is inside the triangle
bool pointInTriangle(const sf::Vector2f& pt, const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3) 
{
    // Use barycentric coordinates method to check
    float d1 = sign(pt, v1, v2);
    float d2 = sign(pt, v2, v3);
    float d3 = sign(pt, v3, v1);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void triangulatePolygon(sf::VertexArray &vertices, const std::vector<sf::Vector2f> &polygon, const sf::Color &color)
{
    assert(polygon.size() >= 3);

    std::vector<sf::Vector2f> remainingVertices = polygon;

    while (remainingVertices.size() > 3) 
    {
        bool earFound = false;

        // Check every vertex to find an ear
        for (auto it = remainingVertices.begin(); it != remainingVertices.end(); ++it) 
        {
            auto prev = (it == remainingVertices.begin()) ? remainingVertices.end() - 1 : it - 1;
            auto next = (it + 1 == remainingVertices.end()) ? remainingVertices.begin() : it + 1;

            // Check if vertex is an ear (no other vertices inside the triangle)
            bool isEar = true;
            sf::Vector2f a = *prev;
            sf::Vector2f b = *it;
            sf::Vector2f c = *next;

            sf::Vector2f ab = b - a;
            sf::Vector2f bc = c - b;
            float crossProduct = ab.x * bc.y - ab.y * bc.x; // Cross product to determine winding

            if (crossProduct < 0) 
            {
                isEar = false; // Concave angle, not an ear
            }
            else {
                for (auto check = remainingVertices.begin(); check != remainingVertices.end(); ++check) 
                {
                    if (check != prev && check != it && check != next) 
                    {
                        if (pointInTriangle(*check, a, b, c)) 
                        {
                            isEar = false;
                            break;
                        }
                    }
                }
            }

            if (isEar) 
            {
                vertices.append(a);
                vertices.append(b);
                vertices.append(c);

                // Remove ear vertex.
                remainingVertices.erase(it);
                earFound = true;

                break; 
            }
        }

        if (!earFound) {
            // No more ears found, stop the triangulation (should not happen for simple polygons).
            break;
        }
    }

    // Last triangle.
    if (remainingVertices.size() == 3) 
    {
        vertices.append(remainingVertices[0]);
        vertices.append(remainingVertices[1]);
        vertices.append(remainingVertices[2]);
    }
}

void appendVertexArray(sf::VertexArray& targetArray, const sf::VertexArray& sourceArray)
{
	for (int i = 0; i < sourceArray.getVertexCount(); ++i)
	{
		targetArray.append(sourceArray[i]);
	}
}
