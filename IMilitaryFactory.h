#pragma once
#include <memory>

class LandArmy;
class NavalFleet;

class IMilitaryFactory
{
public:
	~IMilitaryFactory() = default;

	// Creates land army on some territory and returns handle to army.
	virtual std::unique_ptr<LandArmy> yieldLandArmy() = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() = 0;

};