#pragma once
#include <vector>
#include <map>
#include "Title.h"

class Player;
class PlayerEstateManager;
class VassalManager;
class Estate;
class Barony;

/*
 * Handles the allocation of estates to the ruler and their vassals. 
 */
class EstateAllocator
{
public:
	EstateAllocator(Player &ruler, PlayerEstateManager &rulerEstateManager, VassalManager &vassalManager);

	// Confers the estate to ruler or a vassal. Returns the player estate is conferred to.
	// This function does not update the estates ownership. It should only be called by the 
	// estate in question so the estate itself can update its ownership.
	Player& allocate(Estate &estate);

private:
	Player &ruler;
	PlayerEstateManager &rulerEstateManager; 
	VassalManager &vassalManager;  
	// Limit on rulers directly controlled baronies.
	int rulerBaronyLimit = 3;  
	// Limit on rulers controlled unlanded estates. Estate with title greater than realms
	// highest estate title is automatically granted to ruler despite limit.
	int rulerUnlandedLimit = 7;  
	// Limit on rulers number of vassals.
	int rulerVassalLimit = 14;  
};

// Returns the player with the greatest influence over the specified barony from a parameter vector of players.
Player &getGreatestBaronyInfluence(const Barony &barony, const std::vector<Player*> &players);

// Returns the player with the greatest influence over the specified unlanded estate from a parameter vector of players.
Player &getGreatestUnlandedEstateInfluence(const Estate &estate, const std::vector<Player*> &players);

// Computes the number of unlanded estates from an estate count map.
int countUnlandedEstates(std::map<Title, int> &estateCounts);