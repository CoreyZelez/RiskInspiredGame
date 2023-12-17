#pragma once
#include "HashFunctions.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>

class Player;
class Game;
class Territory;
class LandArmy;
class NavalFleet;

/* Provides information to be used for AI decision making.*/
class PlayerAIContext
{
public:
	PlayerAIContext(Player &player, Game &game);

	/*
	Returns vector of owned territories that lie on player's border.
	*/
	std::vector<Territory*> getBorderTerritories();

	/*
	Returns adjacent territories to territory that are controlled by an enemy player.
	*/
	const std::set<Territory*> getEnemyAdjacencies(Territory &territory, bool includeNeutral = false);

	/* 
	Returns a map of enemy players to int values. Integer values represent
	a threat score of the enemy player's armies to the territory. Closer and 
	larger enemy militaries contributed greater values to the score.
	*/
	std::map<const Player*, int> getArmyWeightedThreats(const Territory &territory);

	/*
	Returns a map of enemy players to int values. Integer values represent
	a threat score of the enemy player's fleets to the territory. Closer and
	larger enemy fleets contributed greater values to the score.
	*/
	std::map<const Player*, int> getFleetWeightedThreats(const Territory &territory);

	/*
	Returns a map of pairs of territory and distance to a vector of land armies.
	*/
	std::unordered_map<std::pair<const Territory*, int>, std::vector<LandArmy*>, PairTerritoryIntHash> getArmyBorderDistances(int maxDist);

	/*
	Returns a map of pairs of territory and distance to a vector of naval fleets.
	*/
	std::unordered_map<std::pair<const Territory*, int>, std::vector<NavalFleet*>, PairTerritoryIntHash> getFleetBorderDistances(int maxDist);

private:
	Player &player;
	Game &game;
	int maxDist = 13;
};

// Returns the distance between the two territories using bfs strictly traversing only
// territories owned by the owner of territory1 and territory2.
int calculateFriendlyDistanceBFS(const Territory &territory1, const Territory& territory2, int maxDist);

// Returns the distance between the source territory and the target territories in an unordered_map. Only allows 
// traversal between territories owned by source territory owner.
std::unordered_map<const Territory*, int> calculateFriendlyDistancesBFS(const Territory &sourceTerritory, const std::unordered_set<const Territory*> &territories, int maxDist);

int calculateArmyWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor);
int calculateFleetWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor);
int calculateMaxThreat(const std::map<const Player*, int> &threats);
int calculateTotalThreat(const std::map<const Player*, int> &threats);