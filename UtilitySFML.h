#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Check if a point is inside a triangle.
bool isPointInsideTriangle(const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c);

// Check if a point is an ear of a polygon.
bool isEar(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const std::vector<sf::Vector2f>& polygon);

// Triangulate a polygon and store in a vertex array with a desired color.
void triangulatePolygon(sf::VertexArray& vertices, const std::vector<sf::Vector2f>& polygon, const sf::Color& color);

void appendVertexArray(sf::VertexArray& targetArray, const sf::VertexArray& sourceArray);


