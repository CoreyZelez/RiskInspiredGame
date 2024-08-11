#pragma once
#include "Estate.h"
#include "Observer.h"
#include "LandTerritory.h"
#include "NavalTerritory.h"

class Territory;

template<typename T> class LandedEstate : public Estate
{
public:
	LandedEstate(Title title, T& territory, sf::Color color);

	virtual ~LandedEstate() = default;

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield() override;

	// Creates land army on some territory and returns handle to army.
	virtual std::unique_ptr<LandArmy> yieldLandArmy() = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() = 0;

	virtual bool containsPosition(const sf::Vector2f &position) const override;
	virtual bool isLanded() const override;

	virtual std::vector<const Territory*> getTerritories() const override;
	virtual std::vector<Territory*> getTerritories() override;

	int getGridId() const;

	T& getTerritory();
	const T& getTerritory() const;

protected:
	T& territory;

	// Yields military units at territory if possible.
	virtual void generateMilitary(MilitaryManager &militaryManager) override;
	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override = 0;

	virtual std::string getSaveLabel() const override = 0;

	// Puts new land army owned by ruler onto territory. Returns handle.
	virtual std::unique_ptr<LandArmy> putArmy(int strength);
	// Puts new naval fleet owned by ruler onto territory. Returns handle.
	virtual std::unique_ptr<NavalFleet> putFleet(int strength);
};



