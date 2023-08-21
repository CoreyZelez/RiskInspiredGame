#pragma once
#include <vector>

class Player;

class PlayerRelationshipManager
{
public:
	explicit PlayerRelationshipManager(const Player &player);

	// void receiveVassal
	// void transferVassal
	// Adds player as a vassal. Requires player has no liege.
	void addVassal(Player &vassal);
	bool isVassal(const Player &player) const;
	bool hasLiege() const;

private:
	const Player &player;
	const Player *liege;
	std::vector<Player*> vassals;
};

