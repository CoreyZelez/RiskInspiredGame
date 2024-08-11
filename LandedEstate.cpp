#pragma once
#include "LandedEstate.h"
#include "Territory.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"
#include "NavalFleet.h"
#include "LandArmy.h"
#include "MilitaryManager.h"
#include "LandTerritory.h"
#include "Player.h"
#include <fstream>
#include <iostream>

template<typename T>
LandedEstate<T>::LandedEstate(Title title, T& territory, sf::Color color)
	: Estate(title, territory.getGrid(), color), territory(territory)
{
}

template<typename T>
void LandedEstate<T>::yield()
{
	// Yields army if able to territory associated with estate.
	std::unique_ptr<LandArmy> landArmy = yieldLandArmy();
	// Yields fleet if able to territory associated with estate.
	std::unique_ptr<NavalFleet> navalFleet = yieldNavalFleet();
	if(landArmy != nullptr)
	{
		landArmy.get()->getOwner().getMilitaryManager().addLandArmy(std::move(landArmy));
	}
	if(navalFleet != nullptr)
	{
		navalFleet.get()->getOwner().getMilitaryManager().addNavalFleet(std::move(navalFleet));
	}
}

template<typename T>
bool LandedEstate<T>::containsPosition(const sf::Vector2f &position) const
{
	return territory.getGrid().containsPosition(position);
}

template<typename T>
bool LandedEstate<T>::isLanded() const
{
	return true;
}

template<typename T>
std::vector<const Territory*> LandedEstate<T>::getTerritories() const
{
	return { &territory };
}

template<typename T>
std::vector<Territory*> LandedEstate<T>::getTerritories()
{
	return { &territory };
}

template<typename T>
int LandedEstate<T>::getGridId() const
{
	// Landed estates have only a single subgrid in the associated composite grid.
	assert(getGrid().getGridIds().size() == 1);
	return getGrid().getGridIds()[0];
}

template<typename T>
T& LandedEstate<T>::getTerritory()
{
	return territory;
}

template<typename T>
const T& LandedEstate<T>::getTerritory() const
{
	return territory;
}

template<typename T>
void LandedEstate<T>::generateMilitary(MilitaryManager &militaryManager)
{
}

template<typename T>
std::unique_ptr<LandArmy> LandedEstate<T>::putArmy(int strength)
{
	// Territory must be controlled by player putting army.
	assert(territory.getUpperController() != nullptr);
	assert(territory.getUpperController() == getRuler());
	assert(territory.getOccupancyHandler()->getArmy() == nullptr || &territory.getOccupancyHandler()->getArmy()->getOwner() == getRuler());

	// Yield army to upper liege.
	Player &upperLiege = getRuler()->getUpperLiege();
	std::unique_ptr<LandArmy> army = std::make_unique<LandArmy>(upperLiege, &territory, strength);
	territory.getOccupancyHandler()->occupy(army.get());

	// Army merged with pre-existing army on territory.
	if(army->isDead())
	{
		return nullptr;
	}

	// There was no pre-existing army on territory.
	assert(army.get()->getTotalStrength() > 0);
	return army;
}

template<typename T>
std::unique_ptr<NavalFleet> LandedEstate<T>::putFleet(int strength)
{
	// Intentionally does nothing.
	return nullptr;
}

