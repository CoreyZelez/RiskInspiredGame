#pragma once
#include "Estate.h"
#include "Territory.h"
#include "IOccupy.h"

/*
 * DirectEstate refers to an estate with no lower titles. Necessarily associated with a territory.
 */
template<class T> class DirectEstate : public Estate, public IOccupy
{
public:
	DirectEstate(std::unique_ptr<T> territory)
		: territory(std::move(territory))
	{}

	virtual bool attemptOccupy(std::shared_ptr<LandArmy> &army) override
	{
		return territory.get()->attemptOccupy(army);
	}

	virtual bool attemptOccupy(std::shared_ptr<NavalFleet> &fleet) override
	{
		return territory.get()->attemptOccupy(fleet);
	}

	std::unique_ptr<T> &getTerritory()
	{
		return territory;
	}

private:
	std::unique_ptr<T> territory;  
};

