#pragma once
#include <unordered_set>
#include <map>

class Player;

class PlayerDiplomacy
{
public:
	PlayerDiplomacy(Player &player);

	void update();

	// Adds attacking player to attackHistory.
	void addAttackHistory(Player &enemy);

private:
	// Player diplomacy is managed for.
	Player &player;
	// Tracks number of turns (up to 10) since another player last attacked this player. 
	std::map<int, std::unordered_set<Player*>> attackHistory;
	// Maximum number of turns which history of attacks is kept for.
	const int maxHistory = 10;
};

