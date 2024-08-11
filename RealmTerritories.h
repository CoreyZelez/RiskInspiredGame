#pragma once
#include <unordered_set>

class Territory;
class RealmGrid;

class RealmTerritories
{
public:
	RealmTerritories(RealmGrid& grid);

	void eraseTerritories(const RealmTerritories& territories);
	void eraseTerritory(Territory& territory);

	void addControl(Territory& territory);
	void removeControl(const Territory& territory);

	// Adds territory that belongs to estate of realm 
	void addEstateTerritory(Territory& territory);
	void removeEstateTerritory(const Territory& terriory);

	// Adds a territory, specifying it is under control and currently sieging the associated barony.
	void addSiegingTerritory(Territory& territory);
	void removeSiegingTerritory(const Territory& territory);

	// Specify that a previously controlled territory has come under siege.
	void addSiegedTerritory(Territory& territory);

	const std::unordered_set<Territory*>& getControlledTerritories() const;
	const std::unordered_set<Territory*>& getControlledEstateTerritories() const;
	const std::unordered_set<Territory*>& getEstateTerritories() const;
	const std::unordered_set<Territory*>& getSiegingTerritories() const;
	const std::unordered_set<Territory*>& getSiegedTerritories() const;

	bool controlsTerritory(const Territory& territory) const;

	bool testInvariants() const;

private:
	std::unordered_set<Territory*> controlledTerritories;  // All territories of realm. Includes sieging territories.
	std::unordered_set<Territory*> controlledEstateTerritories;  // All territories of realm. Includes sieging territories.
	std::unordered_set<Territory*> estateTerritories;  // All territories of realm associated with estate of realm.
	std::unordered_set<Territory*> siegingTerritories;  // Enemy territories currently sieging.
	std::unordered_set<Territory*> siegedTerritories;  // Territories associated with barony in realm currently under siege.
	RealmGrid& grid;
};

// Returns true if no controlled territory is shared by both instances.
bool territoriesDisjoint(const RealmTerritories& territories1, const RealmTerritories& territories2);

