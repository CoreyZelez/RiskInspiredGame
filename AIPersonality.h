#pragma once

struct AIPersonality
{
	int aggression;  // Preference to play defensively vs offensively.
	int invadeSmallLargePreference;  // Preference towards invading small vs large enemy realms.
	int weakStrongVassalPreference;  // Preference towards granting fiefs to weak vs strong vassals.
	int vassalTaxPreference;  // Preference to tax vassals less vs more.
	int dejurePreference;  // Preference to target dejure baronies of owned estates and parent estates.
};

