#pragma once

struct LiegePolicy;
class Player;

struct VassalPolicy
{
	VassalPolicy(const Player &player);

	const Player &player;

	// Adjusts liege influence.
	void handleLiegeInfluenceChange(const LiegePolicy &liegePolicy);
	// Adjusts resistance against liege.
	void handleResistanceChange(const Player &liege, const LiegePolicy &liegePolicy);

	bool canRebel() const;

	// Percent of generated troops granted to liege.
	double liegeLevyContribution = 0.3;

	// Influence over liege for estate conferral. 
	double liegeInfluence = 0;  
	// Resistance score. When exceeding threshold, may rebel against liege.
	double resistance = 0;
	// Threshold which when surpassed by resistance, vassal may choose to rebel.
	const double rebellionThreshold = 100;
};

