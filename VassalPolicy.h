#pragma once
#include "VassalResistance.h"

struct LiegePolicy;
class Player;

struct VassalPolicy
{
	VassalPolicy(Player &player);

	// Adjusts liege influence.
	void handleLiegeInfluenceChange(const LiegePolicy &liegePolicy);

	const Player &player;
	double liegeLevyContribution = 0.4;	 // Percent of generated troops granted to liege.
	double liegeInfluence = 0; // Influence over liege for estate conferral. 
	VassalResistance vassalResistance;
};

