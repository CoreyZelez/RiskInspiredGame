#pragma once
#include "Territory.h"
#include "Subject.h"
#include <memory>

class LandArmy;
class NavalFleet;
class TerritoryEstate;

class LandTerritory : public Territory, public Subject
{
public:
	~LandTerritory() = default;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) override;
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if attempt is successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) override;

	// Put army on territory.
	virtual void putArmy(std::shared_ptr<LandArmy> &army) override;
	// Put fleet on territory.
	virtual void putFleet(std::shared_ptr<NavalFleet> &fleet) override;

	const Player &getOccupant() const;

	bool isCoastal() const;

	// FUNCTION FOR TESTING PURPOSES ONLY
	const std::shared_ptr<LandArmy>& getArmy() const;

private:
	std::shared_ptr<LandArmy> army;  // Army occupying territory. 
	std::shared_ptr<NavalFleet> fleet;  // Fleet occupying coast assuming isCoastal true. 
	// In future above will probably be vector so multiple different occupants e.g. friendly nations or vassals.
};