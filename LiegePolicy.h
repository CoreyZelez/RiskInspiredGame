#pragma once

struct LiegePolicy
{
	// Percent of vassal troops requested by liege.
	double vassalLevy = 0.6;

	// Weight for conferral of baronies to vassals considering vassal realm size. Higher weight prioritises smaller realmed vassals.
	double baronyConferralRealmSizeWeight = 0.1;
	// Weight for conferral of baronies to vassals considering liege influence of vassal.
	double baronyConferralLiegeInfluenceWeight = 0.5;
	// Weight for conferral of baronies to vassals considering vassal's controlled related estates.
	double baronyConferralRelatedEstatesWeight = 0.4;

	// Limit on rulers directly controlled baronies.
	int rulerBaronyLimit = 4;
	// Limit on rulers controlled unlanded estates. Estate with title greater than realms
	// highest estate title is automatically granted to ruler despite limit.
	int rulerUnlandedLimit = 6;
	// Limit on rulers number of vassals.
	int rulerVassalLimit = 14;
};

