#pragma once
#include <unordered_set>

class Territory;

class RealmTerritories
{
public:
	void removeTerritories(const RealmTerritories& territories);
	void removeTerritory(const Territory& territory);
	// Adds territory that belongs to estate of realm and is controlled. 
	void addControlledEstateTerritory(Territory& territory);
	// Adds a territory, specifying it is under control and currently sieging the associated barony.
	void addSiegingTerritory(Territory& territory);
	// Specify that a previously controlled territory has come under siege.
	void addSiegedTerritory(Territory& territory);

	const std::unordered_set<Territory*>& getControlledTerritories() const;
	const std::unordered_set<Territory*>& getControlledEstateTerritories() const;
	const std::unordered_set<Territory*>& getSiegingTerritories() const;
	const std::unordered_set<Territory*>& getSiegedTerritories() const;

	bool controlsTerritory(const Territory& territory) const;

private:
	std::unordered_set<Territory*> controlledTerritories;  // All territories of realm. Includes sieging territories.
	std::unordered_set<Territory*> estateTerritories;  // All territories of realm associated with estate of realm.
	std::unordered_set<Territory*> controlledEstateTerritories;  // All controlled territories of realm associated with estate of realm.
	std::unordered_set<Territory*> siegingTerritories;  // Enemy territories currently sieging.
	std::unordered_set<Territory*> siegedTerritories;  // Territories associated with barony in realm currently under siege.
};

// Returns true if no controlled territory is shared by both instances.
bool territoriesDisjoint(const RealmTerritories& territories1, const RealmTerritories& territories2);

