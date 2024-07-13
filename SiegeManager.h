#pragma once
#include "Observer.h"

class Barony;
class LandArmy;

class SiegeManager : public Observer
{
public:
	SiegeManager(Barony& barony, double resistanceStrength = 0);

	virtual void update(Message message) override;

	void update();

	void initSiege(LandArmy& army);

	bool activeSiege() const;

private:
	double resistanceStrength;
	Barony& barony;
	LandArmy* siegingArmy = nullptr;

	// Army vacates, ending siege of barony. Barony ownership remains unchanged.
	void siegeLifted();

	void removeSiegingArmy();
};

