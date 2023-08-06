#pragma once
#include "Territory.h"
#include "Subject.h"
#include <memory>

class TerritoryEstate;

class LandTerritory : public Territory  // Perhaps Territory should be Subject!!! Too much code reptetition...
{
public:
	explicit LandTerritory(TerritoryGraphics graphics);
	LandTerritory();
	~LandTerritory() = default;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) override;
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if attempt is successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) override;

	const Player &getOccupant() const;

	const std::shared_ptr<LandArmy> &getArmy() const;

protected:
	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

private:
	std::shared_ptr<LandArmy> army;  // Army occupying territory. 
};