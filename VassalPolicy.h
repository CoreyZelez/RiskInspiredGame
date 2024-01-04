#pragma once

struct LiegePolicy;

struct VassalPolicy
{
	// Adjusted liege influence based upon forfillment of vassal levy asked by players liege.
	void handleLiegeInfluenceChange(const LiegePolicy &liegePolicy);

	// Percent of generated troops granted to liege.
	double liegeLevyContribution = 0.7;

	// Influence over liege for estate conferral. 
	double liegeInfluence = 0;  
	// Resistance score. When exceeding threshold, may rebel against liege.
	double resistance = 0;
};

