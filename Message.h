#pragma once

enum Message
{
	newController,  // Signifys occupant changed.
	vacatedMilitary,  // Signifys military unit vacated its location, either due to movement or death.

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