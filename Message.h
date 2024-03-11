#pragma once

enum Message
{
	newOccupant,  // Signifys occupant changed.
	deadMilitary,  // Signifys death of military unit.

	lostBarony, 
	receivedBarony,
	lostCounty,
	receivedCounty,
	lostDuchy,
	receivedDuchy,
	lostKingdom,
	receivedKingdom,
	lostEmpire,
	receivedEmpire
};