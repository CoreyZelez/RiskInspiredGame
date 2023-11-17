#pragma once
#include <vector>
#include <map>
#include <set>

class Player;
class Game;
class Territory;
class LandArmy;

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
	const std::set<Territory*> getEnemyAdjacencies(Territory &territory);

	/* 
	Returns a map of enemy players to int values. Integer values represent
	a threat score of the enemy player's armies to the territory. Closer and 
	larger enemy militaries contributed greater values to the score.
	*/
	std::map<const Player*, int> getWeightedThreats(const Territory &territory);

	/*
	Returns a map of pairs of territory and distance to a vector of land armies
	*/
	std::map<std::pair<const Territory*, int>, std::vector<LandArmy*>> getArmyBorderDistances(int maxDist);

private:
	Player &player;
	Game &game;
	int maxDist = 13;
};

/*
	Returns the distance between the two territories using bfs strictly traversing only
	territories owned by the owner of territory1 and territory2.
	*/
int calculateFriendlyDistanceBFS(const Territory &territory1, const Territory& territory2, int maxDist);
int calculateWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor);
int calculateMaxThreat(const std::map<const Player*, int> &threats);
int calculateTotalThreat(const std::map<const Player*, int> &threats);