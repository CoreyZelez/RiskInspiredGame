#pragma once
#include "Factory.h"
#include "Factory.cpp"
#include <SFML/Graphics.hpp>
#include <map>

class TerritoryMaker;
class EditorTerritory;

// Define a function pointer type
template<typename T>
using LandFeatureAdjustmentFunction = void(*)(EditorTerritory&, const T&);

template<typename T> class LandTerritoryFeatureEditor
{
public:
	LandTerritoryFeatureEditor(LandFeatureAdjustmentFunction<T> adjustmentFunction, TerritoryMaker &territoryMaker, Factory<int, T> factory);

	void handleInput(const sf::RenderWindow &window);

private:
	T selected;  // Selected feature variation.
	Factory<int, T> factory;
	std::map<sf::Keyboard::Key, int> keyIDMap;  // Maps key to feature variation index.
	TerritoryMaker &territoryMaker;
	LandFeatureAdjustmentFunction<T> adjustmentFunction;

	void handleKeyPress(sf::Keyboard::Key key);
};




