#include "EstateUnitTest.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Player.h"
#include "Game.h"

void EstateUnitTest::test()
{
	testBaronyOwnership();
	testBaronyYield();
}

void EstateUnitTest::testBaronyOwnership()
{
	// Variable to hold message of current error being tested.
	std::string testName;
	std::string failMessage;

	Game game("empty");
	Player player(game, "");
	LandTerritoryFeatures landTerritoryFeatures(&game.getGameplaySettings());
	landTerritoryFeatures.coreProsperity = getHighProsperityThreshold();
	landTerritoryFeatures.prosperity = getHighProsperityThreshold();
	LandTerritory territory(0);
	Barony barony(territory);

	// Operation being tested.
	barony.setOwnership(&player);

	// First test.
	testName = "Barony has correct ruler.";
	failMessage = "Barony ruler incorrect";
	bool correctRuler = barony.getRuler() == &player;
	testReport(testName, correctRuler, failMessage);

	// Second test.
	testName = "Player realm contains barony.";
	failMessage = "Player realm does not contain barony.";
	bool realmContainsBarony = *player.getRealm().getEstates().begin() == &barony;
	testReport(testName, correctRuler, failMessage);

	// Second operation being tested.
	barony.setOwnership(nullptr);

	// Third test.
	testName = "Barony ruler removed.";
	failMessage = "Barony ruler not removed";
	bool rulerRemoved = barony.getRuler() == nullptr;
	testReport(testName, rulerRemoved, failMessage);

	// Fourth test.
	testName = "Player realm does not contain barony.";
	failMessage = "Barony not removed from player realm.";
	bool realmNotContainsBarony = player.getRealm().getEstates().size() == 0;
	testReport(testName, realmNotContainsBarony, failMessage);
}

void EstateUnitTest::testBaronyYield()
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
	std::unordered_set<sf::Vector2i, Vector2iHash> landPositions = { sf::Vector2i(0, 0) };
	std::unordered_set<sf::Vector2i, Vector2iHash> navalPositions = { sf::Vector2i(0, 1) };
	EditorGrid landGrid(landPositions);
	EditorGrid navalGrid(navalPositions);
	// Ensure territories adjacent for successful port creation.
	landGrid.addPosition(sf::Vector2i(0, 0));
	navalGrid.addPosition(sf::Vector2i(1, 0));
	LandTerritoryFeatures landTerritoryFeatures(&game.getGameplaySettings());
	landTerritoryFeatures.coreProsperity = getHighProsperityThreshold();
	landTerritoryFeatures.prosperity = getHighProsperityThreshold();
	LandTerritory landTerritory(landID, landGrid, landTerritoryFeatures);
	NavalTerritory navalTerritory(navalID, navalGrid);
	std::vector<Territory*> territories = { &navalTerritory };
	landTerritory.getDistanceMap().addAdjacencies(territories);
	// Create the barony.
	landTerritory.createPort(navalTerritory);
	Barony barony(landTerritory);
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
		bool testArmyStrength = actualStrength > 0;
		failMessage = "Land army not yielded.";
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
		testName = "Naval fleet yielded.";
		const int actualStrength = navalTerritory.getOccupancyHandler()->getFleet()->getTotalStrength();
		bool testFleetStrength = actualStrength > 0;
		failMessage = "Naval fleet not yielded.";
		testReport(testName, testFleetStrength, failMessage);
	}
}

