#pragma once
#include "Territory.h"
#include "Observer.h"
#include "Subject.h"
#include <memory>

class NavalFleet;
class LandArmy;

class LandTerritory : public Territory 
{
public:
	LandTerritory(int id, Grid graphics);
	explicit LandTerritory(int id);

	virtual ~LandTerritory() = default;

	// Calculates distance to each territory in territories and updates distances map.
	virtual void calculateDistances(const std::vector<Territory*> &territories) override;
	virtual int getDistance(const Territory &territory, bool sameType) const override;
	virtual const std::set<Territory*> &getAdjacencies(bool sameType) const override;
	virtual std::set<Territory*> &getAdjacencies(bool sameType) override;
	virtual void addAdjacencies(std::vector<Territory*> &territories) override;

	void setIsCoastal(bool isCoastal);
	bool getIsCoastal() const;

	// Save label is identifier in txt file for territory type.
	virtual std::string getSaveLabel() const override;

private:
	bool isCoastal = false;
	// Distances between other land territories.
	mutable std::map<const Territory*, int> landTerritoryDistances;
	// Adjacent land territories.
	std::set<Territory*> landTerritoryAdjacencies;

};