#pragma once
#include <memory>

class LandArmy;
class NavalFleet;

class IMilitaryFactory
{
public:
	~IMilitaryFactory() = default;

	// Creates land army on some territory and returns handle to army.
	virtual std::shared_ptr<LandArmy> yieldLandArmy() = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::shared_ptr<NavalFleet> yieldNavalFleet() = 0;

};