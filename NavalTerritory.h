#pragma once
#include "Territory.h"

class NavalTerritory : public Territory
{
public:
	NavalTerritory();
	~NavalTerritory() = default;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) override;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) override;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) override;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) override;
private:

};

