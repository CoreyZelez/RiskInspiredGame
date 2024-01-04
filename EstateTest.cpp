#include "EstateTest.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Player.h"
#include "Game.h"

void EstateTest::test()
{
	testBaronyYield();
}

void EstateTest::testBaronyYield()
{
	// Variable to hold message of current error being tested.
	std::string testName;
	std::string failMessage;

	Game game("empty");
	Player player(game, "");

	// Construct territories.
	const int landID = 1;
	const int navalID = 2;
	// Construct grids using "test" constructor.
	std::unordered_set<sf::Vector2i, Vector2iHash> landPositions = { sf::Vector2i(0,0) };
	std::unordered_set<sf::Vector2i, Vector2iHash> navalPositions = { sf::Vector2i(0,1) };
	Grid landGrid(landPositions);
	Grid navalGrid(navalPositions);
	// Ensure territories adjacent for successful port creation.
	landGrid.addPosition(sf::Vector2i(0, 0));
	navalGrid.addPosition(sf::Vector2i(1, 0));
	LandTerritory landTerritory(landID, landGrid);
	NavalTerritory navalTerritory(navalID, navalGrid);
	std::vector<Territory*> territories = { &navalTerritory };
	landTerritory.getDistanceMap().addAdjacencies(territories);
	// Create the port.
	landTerritory.createPort(navalTerritory);
	// Create the barony with desired yields.
	const int armyYield = 10;
	const int fleetYield = 5;
	Barony barony(landTerritory, armyYield, fleetYield);
	barony.setOwnership(&player);

	// Operation being tested.
	barony.yield();

	// First test.
	testName = "Land army not nullptr.";
	bool testArmyYielded = landTerritory.getOccupancyHandler()->getArmy() != nullptr;
	failMessage = "Land army was nullptr.";
	testReport(testName, testArmyYielded, failMessage);

	// Second test.
	if(testArmyYielded)
	{
		testName = "Correct land army strength yield.";
		const int actualStrength = landTerritory.getOccupancyHandler()->getArmy()->getTotalStrength();
		bool testArmyStrength = actualStrength == armyYield;
		failMessage = "Expected strength is " + std::to_string(armyYield) +
			". Actual strength is " + std::to_string(actualStrength) + ".";
		testReport(testName, testArmyStrength, failMessage);
	}

	// Third test.
	testName = "Naval fleet not nullptr.";
	bool testFleetYielded = navalTerritory.getOccupancyHandler()->getFleet() != nullptr;
	failMessage = "Naval fleet was nullptr.";
	testReport(testName, testFleetYielded, failMessage);

	// Fourth test.
	if(testFleetYielded)
	{
		testName = "Correct naval fleet strength yield.";
		const int actualStrength = navalTerritory.getOccupancyHandler()->getFleet()->getTotalStrength();
		bool testFleetStrength = actualStrength == fleetYield;
		failMessage = "Expected strength is " + std::to_string(fleetYield) +
			". Actual strength is " + std::to_string(actualStrength) + ".";
		testReport(testName, testFleetStrength, failMessage);
	}
}

