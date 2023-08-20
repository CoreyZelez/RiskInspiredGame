#include "EstateTest.h"
#include "Barony.h"
#include "Territory.h"
#include "LandArmy.h"
#include "Player.h"

void EstateTest::test()
{
	test1();
}

void EstateTest::test1()
{
	std::string testName = "barony yielding land army";
	
	Player player;
	LandTerritory territory(0);
	const double landArmyYield = 0.4;
	std::shared_ptr<Barony> barony = std::make_shared<Barony>(territory, landArmyYield);
	barony.get()->initRuler(player);
	
	// Attempt to yield land army with cumulative land army not surpassing threshold.
	for(int i = 0; i < 3; ++i)
	{
		player.handleFiefYields();
	}
	if(barony.get()->yieldLandArmy() != nullptr)
	{
		bool result = false;
		std::string message = "unexpected land army yielded";
		testReport(testName, result, message);
		return;
	}
	
	// Attempts to successfully yield a land army.
	for(int i = 0; i < 6; ++i)
	{
		player.handleFiefYields();
	}
	if(territory.getArmy().get() == nullptr)
	{
		bool result = false;
		std::string message = "no land army yielded to territory";
		testReport(testName, result, message);
		return;
	}
	else if(&territory.getArmy().get()->getOwner() != &player)
	{
		bool result = false;
		std::string message = "incorrect owner of yielded land army";
		testReport(testName, result, message);
		return;
	}
	
	// Test succeeded.
	bool result = true;
	testReport(testName, result);
}
