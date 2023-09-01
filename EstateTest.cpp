#include "EstateTest.h"
#include "Barony.h"
#include "LandTerritory.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "Player.h"
#include "Game.h"

void EstateTest::test()
{
	test1();
	test2();
}

void EstateTest::test1()
{
	std::string testName = "barony yielding land army";
	Game game("");
	AIPersonality personality = { 0 };
	Player player(game, personality);
	LandTerritory territory(0);
	const double landArmyYield = 0.4;
	const double navalFleetYield = 0.1;
	std::unique_ptr<Estate> barony = std::make_unique<Barony>(territory, landArmyYield, navalFleetYield);
	barony.get()->setRuler(&player);

	// Attempt to yield land army with cumulative land army not surpassing threshold.
	player.getRealm().handleFiefYields();
	if(territory.getArmy() != nullptr)
	{
		bool result = false;
		std::string message = "unexpected land army yielded";
		testReport(testName, result, message);
		return;
	}

	// Attempts to successfully yield a land army.
	// 20 loops executed as this should guaruntee surpassing threshold for any sanely chosen threshold value.
	for(int i = 0; i < 20; ++i)
	{
		player.getRealm().handleFiefYields();
	}
	if(territory.getArmy() == nullptr)
	{
		bool result = false;
		std::string message = "no land army yielded to territory";
		testReport(testName, result, message);
		return;
	}
	else if(&territory.getArmy()->getOwner() != &player)
	{
		bool result = false;
		std::string message = "incorrect owner of yielded land army";
		testReport(testName, result, message);
		return;
	}
	else if(territory.getArmy()->getStrength() <= 0)
	{
		bool result = false;
		std::string message = "invalid army strength yielded.";
		testReport(testName, result, message);
		return;
	}

	// Test succeeded.
	bool result = true;
	testReport(testName, result);
}

void EstateTest::test2()
{
	std::string testName = "county providing bonus yields";

	Game game("");
	AIPersonality personality = { 0 };
	Player player1(game, personality);
	Player player2(game, personality);
	Player player3(game, personality);
	LandTerritory territory1(0);
	LandTerritory territory2(1);
	LandTerritory territory3(2);
	LandTerritory territory4(3);

	const double landArmyYield = 0.4;
	const double navalFleetYield = 0.1;
	std::unique_ptr<Estate> barony1 = std::make_unique<Barony>(territory1, landArmyYield, navalFleetYield);
	std::unique_ptr<Estate> barony2 = std::make_unique<Barony>(territory2, landArmyYield, navalFleetYield);
	std::unique_ptr<Estate> barony3 = std::make_unique<Barony>(territory3, landArmyYield, navalFleetYield);
	std::unique_ptr<Estate> barony4 = std::make_unique<Barony>(territory4, landArmyYield, navalFleetYield);
	std::unique_ptr<Estate> county = std::make_unique<Estate>(Title::count);

	county.get()->addSubfief(barony1.get());
	county.get()->addSubfief(barony2.get());
	county.get()->addSubfief(barony3.get());
	county.get()->addSubfief(barony4.get());

	barony1.get()->setRuler(&player1);
	barony2.get()->setRuler(&player1);
	barony3.get()->setRuler(&player2);
	county.get()->setRuler(&player2);
	barony4.get()->setRuler(&player3);  // player3 will not be apart of player2's realm.

	player2.getRealm().addVassal(player1);

	player2.getRealm().handleFiefYields();
	player1.getRealm().handleFiefYields();
	if(territory1.getArmy() != nullptr)
	{
		bool result = false;
		std::string message = "unexpected land army yielded";
		testReport(testName, result, message);
		return;
	}

	// Attempts to successfully yield a land army through yields only generated by bonus yields provided by county to baronies.
	// 150 loops executed as this should guaruntee surpassing yield threshold for any sanely chosen threshold value.
	for(int i = 0; i < 150; ++i)
	{
		player2.getRealm().handleFiefYields();
	}
	player1.getRealm().handleFiefYields();
	if(territory1.getArmy() == nullptr)
	{
		bool result = false;
		std::string message = "no land army yielded to territory";
		testReport(testName, result, message);
		return;
	}
	else if(&territory1.getArmy()->getOwner() != &player1)
	{
		bool result = false;
		std::string message = "incorrect owner of yielded land army";
		testReport(testName, result, message);
		return;
	}

	// Checks that the player not apart of the realm was not receiving the bonus yields.
	player3.getRealm().handleFiefYields();
	if(territory4.getArmy() != nullptr)
	{
		bool result = false;
		std::string message = "player receiving bonus yields from a player in a different realm";
		testReport(testName, result, message);
		return;
	}

	bool result = true;
	testReport(testName, result);
}