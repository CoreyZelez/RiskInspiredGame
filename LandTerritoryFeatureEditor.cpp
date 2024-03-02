#pragma once
#include "LandTerritoryFeatureEditor.h"
#include "InputUtility.h"
#include "LandTerritory.h"

template<typename T>
LandTerritoryFeatureEditor<T>::LandTerritoryFeatureEditor(LandFeatureAdjustmentFunction<T> adjustmentFunction, 
	TerritoryManager &territoryManager, Factory<int, T> factory)
	: adjustmentFunction(adjustmentFunction), territoryManager(territoryManager), factory(factory)
{
	keyIDMap[sf::Keyboard::Key::Num1] = 0;
	keyIDMap[sf::Keyboard::Key::Num2] = 1;
	keyIDMap[sf::Keyboard::Key::Num3] = 2;
	keyIDMap[sf::Keyboard::Key::Num4] = 3;
	keyIDMap[sf::Keyboard::Key::Num5] = 4;
	keyIDMap[sf::Keyboard::Key::Num6] = 5;
	keyIDMap[sf::Keyboard::Key::Num7] = 6;
	keyIDMap[sf::Keyboard::Key::Num8] = 7;
	keyIDMap[sf::Keyboard::Key::Num9] = 8;
	keyIDMap[sf::Keyboard::Key::Num0] = 9;
}

template<typename T>
void LandTerritoryFeatureEditor<T>::handleInput(const sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

	InputUtility &inputUtility = InputUtility::getInstance();

	// Change feature of territory at world position.
	if(inputUtility.getButtonDown(sf::Mouse::Left))
	{
		// Select land territory for feature change.
		LandTerritory *territory = territoryManager.getLandTerritory(worldPos);

		// Change feature.
		if(territory != nullptr)
		{
			(*adjustmentFunction)(*territory, selected);
		}
	}

	// Handle changes to selected feature variation.
	for(auto keyIndex : keyIDMap)
	{
		sf::Keyboard::Key key = keyIndex.first;
		if(inputUtility.getKeyDown(key))
		{
			handleKeyPress(key);
			break;  // Only handle one key press.
		}
	}
}

template<typename T>
void LandTerritoryFeatureEditor<T>::handleKeyPress(sf::Keyboard::Key key)
{
	int id = -1;
	if(keyIDMap.count(key) == 1)
	{
		id = keyIDMap[key];
	}

	if(!factory.has(id))
	{
		return;
	}

	selected = factory.create(id); 
}

