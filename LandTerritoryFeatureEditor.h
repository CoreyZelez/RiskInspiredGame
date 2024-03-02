#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>
#include <map>

class TerritoryManager;
class LandTerritory;

// Define a function pointer type
template<typename T>
using LandFeatureAdjustmentFunction = void(*)(LandTerritory&, const T&);

template<typename T> class LandTerritoryFeatureEditor
{
public:
	LandTerritoryFeatureEditor(LandFeatureAdjustmentFunction<T> adjustmentFunction, TerritoryManager &territoryManager, Factory<int, T> factory);

	void handleInput(const sf::RenderWindow &window);

private:
	T selected;  // Selected feature variation.
	Factory<int, T> factory;
	std::map<sf::Keyboard::Key, int> keyIDMap;  // Maps key to feature variation index.
	TerritoryManager &territoryManager;
	LandFeatureAdjustmentFunction<T> adjustmentFunction;

	void handleKeyPress(sf::Keyboard::Key key);
};




