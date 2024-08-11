#pragma once

class Barony;
class LandArmy;
class Player;
class GameplaySettings;

class SiegeManager
{
public:
	SiegeManager(const GameplaySettings& gameplaySettings, Barony& barony);
	
	// Updates resistance strength when not under siege.
	void update();

	// Updates siege by simulating siege combat and handling barony ownership changes.
	void updateSiege();

	// Handles beginning of sieged due to army occupying territory associated with barony. Returns
	// true if a siege is begun.
	void handleSiegeBegin(LandArmy& army);
	void handleArmyVacated();

	bool activeSiege() const;
	Player& getBaronyRuler();

private:
	double resistanceStrength = 0;
	Barony& barony;
	LandArmy* siegingArmy = nullptr;
	const GameplaySettings& gameplaySettings;

	void updateResistance();

	// Army vacates, ending siege of barony. Barony ownership remains unchanged.
	void siegeLifted();

	void initSiege(LandArmy& army);

	void removeSiegingArmy();

	// Adjusts resistance strength and sieging army strength as a result of siege combat.
	void handleSiegeCombat();
};

