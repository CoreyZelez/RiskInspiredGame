#pragma once

struct LiegePolicy
{
	// Percent of vassal troops requested by liege.
	double vassalLevy = 0.6f;

	// Limit on rulers directly controlled baronies.
	int rulerBaronyLimit = 3;
	// Limit on rulers controlled unlanded estates. Estate with title greater than realms
	// highest estate title is automatically granted to ruler despite limit.
	int rulerUnlandedLimit = 7;
	// Limit on rulers number of vassals.
	int rulerVassalLimit = 14;

};

