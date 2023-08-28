#include "TestRunner.h"
#include "EstateTest.h"
#include "MilitaryTest.h"

void TestRunner::runTests()
{
	EstateTest estateTest;
	MilitaryTest militaryTest;

	estateTest.test();
	militaryTest.test();
}
