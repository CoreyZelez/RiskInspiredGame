#pragma once
#include "Territory.h"

class NavalTerritory : public Territory
{
public:
	NavalTerritory(int id, Grid grid);
	explicit NavalTerritory(int id);

	virtual ~NavalTerritory() = default;

	/// virtual Player *getOccupant() override;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

protected:

private:

};

