#include "MilitaryTest.h"
#include "LandArmy.h"
#include "NavalFleet.h"
#include "LandTerritory.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

void MilitaryTest::test()
{
	test1();
}

void MilitaryTest::test1()
{
	std::string testName = "Army attacking another army and taking over territory.";
	bool result;

	 Game game("");
	 Player player1(game);
	 Player player2(game);
	 LandTerritory territory1(1, sf::Color::White);
	 LandTerritory territory2(2, sf::Color::White);
	 LandArmy army1(player1, &territory1, 30);
	 LandArmy army2(player2, &territory2, 4);
	 territory1.getOccupancyHandler()->occupy(&army1);
	 territory2.getOccupancyHandler()->occupy(&army2);

	 army1.move(territory2, 25);

	 if(territory1.getOccupancyHandler()->getArmy() == nullptr)
	 {
		 result = false;
		 std::string message = "Expected army on territory.";
		 testReport(testName, result, message);
		 return;
	 }

	 // Note that under any sane implementation player1's army should kill player2's army.
	 if(&territory2.getOccupancyHandler()->getArmy()->getOwner() !=  &player1)
	 {
		 result = false;
		 std::string message = "Unexpected owner of territory.";
		 testReport(testName, result, message);
		 return;
	 }

	 if(army1.getTotalStrength() != 5)
	 {
		 result = false;
		 std::string message = "Invalid army strength.";
		 testReport(testName, result, message);
		 return;
	 }

	 // Test succeeded.
	 result = true;
	 testReport(testName, true);
}
