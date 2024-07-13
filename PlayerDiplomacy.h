#pragma once
#include <unordered_set>
#include <map>
#include <memory>
#include <vector>

class Player;

class PlayerDiplomacy
{
public:
	PlayerDiplomacy(Player &player);

	~PlayerDiplomacy();

	void update();

	// Sets colors of players to represent diplomatic relation.
	void setColors(const std::vector<std::unique_ptr<Player>> &players);

	// Removes all diplomatic relations recorded with specified player.
	void removeDiplomacyWithPlayer(Player &player);

	// Adds attacking player to attackHistory.
	void addAttackHistory(Player &enemy);

	// Adds rebelled player to rebellingVassals and attackHistory.
	void addRebelledVassal(Player &rebellingVassal);

private:
	// Player diplomacy is managed for.
	Player &player;
	// Tracks number of turns (up to 10) since another player last attacked this player. 
	std::map<int, std::unordered_set<Player*>> attackHistory;
	// Vassals currently in rebellion of player. Removed when no longer in attack history.
	std::unordered_set<Player*> rebelledVassals;
	// Maximum number of turns which history of attacks is kept for.
	const int maxHistory = 10;
};

