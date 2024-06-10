#include "UnitTestRunner.h"
#include "EstateUnitTest.h"
#include "GridUnitTest.h"

void UnitTestRunner::runTests()
{
	GridUnitTest gridUnitTest;
	EstateUnitTest estateUnitTest;

	gridUnitTest.test();
	estateUnitTest.test();
}
