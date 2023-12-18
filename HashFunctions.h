#pragma once
#include <utility>
#include <SFML/Graphics.hpp>


class Territory;

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