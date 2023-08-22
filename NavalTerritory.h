#pragma once
#include "Territory.h"

class NavalTerritory : public Territory
{
public:
	explicit NavalTerritory(int id, Grid graphics);
	NavalTerritory(int id);

	~NavalTerritory() = default;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<LandArmy> &army) override;
	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(std::shared_ptr<NavalFleet> &fleet) override;

	/// virtual Player *getOccupant() override;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

protected:

private:

};

