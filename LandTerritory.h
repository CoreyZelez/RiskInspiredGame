#pragma once
#include "Territory.h"
#include "Subject.h"
#include <memory>

class TerritoryEstate;

class LandTerritory : public Territory, public Subject
{
public:
	~LandTerritory() = default;

	void initEstate(const TerritoryEstate *estate);

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) override;
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if attempt is successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) override;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) override;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) override;

	const Player &getOccupant() const;

	const std::shared_ptr<LandArmy> &getArmy() const;

private:
	const TerritoryEstate *estate;  // TerritoryEstate this territory is associated with.

	std::shared_ptr<LandArmy> army;  // Army occupying territory. 
};