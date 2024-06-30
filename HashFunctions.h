#pragma once
#include <utility>
#include <SFML/Graphics.hpp>


class Territory;
class Grid;
enum class TerritoryType;

struct Vector2iHash 
{
	size_t operator()(const sf::Vector2i& vector) const;
};

struct Vector2fHash
{
	size_t operator()(const sf::Vector2f& vector) const;
};

struct PairTerritoryIntHash 
{
	size_t operator()(const std::pair<const Territory*, int>& key) const;
};

struct GridHash
{
	size_t operator()(const Grid& grid) const;
};

struct TerritoryTypeHash
{
	size_t operator()(TerritoryType& type) const;
};

