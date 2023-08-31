#pragma once
#include "Territory.h"
#include "Observer.h"
#include "Subject.h"
#include <memory>

class NavalFleet;
class LandArmy;

class LandTerritory : public Territory, public Observer  // Perhaps Territory should be Subject!!! Too much code reptetition...
{
public:
	LandTerritory(int id, Grid graphics);
	explicit LandTerritory(int id);

	~LandTerritory() = default;

	virtual void update(Message message) override;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(LandArmy *army) override;
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if attempt is successful.
	virtual bool occupy(NavalFleet *fleet) override;

	void setIsCoastal(bool isCoastal);
	bool getIsCoastal() const;
	virtual Player* getOccupant() override;
	const LandArmy* getArmy() const;
	const NavalFleet* getFleet() const;
	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

private:
	// Sets positions of military unit sprites.
	void updateMilitaryPosition(); 

	bool isCoastal = false;
	LandArmy *army;  // Army occupying territory. 
	NavalFleet *fleet;  // Fleet occupying territory coast. 
};