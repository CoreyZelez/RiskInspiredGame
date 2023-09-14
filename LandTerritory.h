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

	virtual ~LandTerritory() = default;

	virtual void update(Message message) override;

	// Army attempts to occupy this territory. Either peaceful or hostile. Returns true if successful.
	virtual bool occupy(LandArmy *army) override;
	// Fleet attempts to occupy this territory. Either peaceful or hostile. Returns true if attempt is successful.
	virtual bool occupy(NavalFleet *fleet) override;

	// Calculates distance to each territory in territories and updates distances map.
	virtual void calculateDistances(const std::vector<Territory*> &territories) override;
	virtual int getDistance(const Territory &territory, bool sameType) const override;
	virtual const std::set<Territory*> &getAdjacencies(bool sameType) const override;
	virtual std::set<Territory*> &getAdjacencies(bool sameType) override;
	virtual void addAdjacencies(std::vector<Territory*> &territories) override;

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
	// Distances between other land territories.
	mutable std::map<const Territory*, int> landTerritoryDistances;
	// Adjacent land territories.
	std::set<Territory*> landTerritoryAdjacencies;
	LandArmy *army;  // Army occupying territory. 
	NavalFleet *fleet;  // Fleet occupying territory coast. 
};