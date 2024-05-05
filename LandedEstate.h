#pragma once
#include "Estate.h"
#include "Observer.h"

class Territory;

class LandedEstate : public Estate, public Observer
{
public:
	LandedEstate(Title title, Territory &territory);
	LandedEstate(Title title, Territory &territory, sf::Color color);

	virtual ~LandedEstate() = default;

	virtual void saveToFile(std::ofstream &file) const override;

	// Handles changing of associated territory occupant.
	virtual void update(Message message) override;

	// Yields any resources directly associated with estate. This does not include subfief resources.
	virtual void yield() override;

	// Returns true if associated land territory has a port.
	bool hasPort() const;

	// Creates land army on some territory and returns handle to army.
	virtual std::unique_ptr<LandArmy> yieldLandArmy() = 0;
	// Creates naval fleet on some territory and returns handle to fleet.
	virtual std::unique_ptr<NavalFleet> yieldNavalFleet() = 0;

	virtual bool containsPosition(const sf::Vector2f &position) const override;

	Territory &getTerritory();

protected:
	// Yields military units at territory if possible.
	virtual void generateMilitary(MilitaryManager &militaryManager) override;
	// Provides bonus yield to this estate.
	virtual void receiveBonusYield(const float &bonus) override = 0;

	virtual std::string getSaveLabel() const override = 0;

	// Puts new land army owned by ruler onto territory. Returns handle.
	virtual std::unique_ptr<LandArmy> putArmy(int strength);
	// Puts new naval fleet owned by ruler onto territory. Returns handle.
	virtual std::unique_ptr<NavalFleet> putFleet(int strength);

private:
	Territory &territory;
};

