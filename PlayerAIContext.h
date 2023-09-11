#pragma once
#include <vector>
#include <map>
#include <set>

class Player;
class Game;
class Territory;

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

private:
	Player &player;
	Game &game;
};

int calculateWeightedThreat(const Territory &territory, const Player &player, const float distanceFactor);
int calculateMaxThreat(const std::map<const Player*, int> &threats);
int calculateTotalThreat(const std::map<const Player*, int> &threats);