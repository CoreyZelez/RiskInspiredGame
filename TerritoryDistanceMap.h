#pragma once
#include <set>
#include <map>
#include <vector>

class Territory;
enum class TerritoryType;

class TerritoryDistanceMap
{
public:
	TerritoryDistanceMap(Territory &territory);

	// Calculates distance to each territory in territories and updates distances map.
	void calculateDistances(const std::vector<Territory*> &territories);

	// Adds territories from parameters that are adjacent (Have bordering grid squares).
	virtual void addAdjacencies(std::vector<Territory*> &territories);

	bool isAdjacent(const Territory *territory) const;

	// Returns true if no adjacent enemy territories.
	bool noEnemyAdjacent(TerritoryType type) const;

	const std::set<Territory*> &getAdjacencies() const;
	std::set<Territory*> &getAdjacencies();

	int getDistance(const Territory &territory) const;

private:
	Territory &territory;
	std::set<Territory*> adjacencies;  // Adjacent territories.
	mutable std::map<const Territory*, int> distances;  // Distances between any territory type.

    // Calculates distances of this territory to all other specified (optionally same type) territories. 
	std::map<const Territory*, int> calculateDistancesBFS(const std::vector<Territory*>& territories);

	// Returns true if territories have touching grid squares.
	bool sharesBorder(const Territory &territory) const;
};

